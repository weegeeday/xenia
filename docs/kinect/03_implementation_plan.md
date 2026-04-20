# Kinect SDK 1.8 → Xbox 360 NUI Implementation Plan

## Overview

This document outlines the strategy for implementing Xbox 360 NUI (Kinect) calls in Xenia
by mapping them to Windows Kinect SDK 1.8 APIs.

## Architecture Decision

### Option A: Runtime SDK Linking (Preferred)
- Optionally link against `Kinect10.lib` / load `Kinect10.dll` at runtime if present.
- When Kinect SDK is not installed, all functions return "device not connected".
- Use `NuiGetSensorCount()` to check if a real sensor is connected.
- This allows users with a real Kinect plugged in to use it in emulated games.

### Option B: Pure Stub
- All functions return "not connected" / `X_ERROR_DEVICE_NOT_CONNECTED`.
- Minimal but games that require Kinect will at least not crash.

### Recommendation
Implement **Option A** as the target, with **Option B** as fallback when SDK is absent.
Since this is Windows-only (Xenia is Windows-only for practical use), this is feasible.

## Implementation Priority

### Phase 1: Baseline "No Device" Stubs (all platforms, no SDK)
All un-implemented NUI functions return safe defaults:
- Device status functions → "not connected"
- Query functions → return 0 / empty
- Setter functions → return success (no-op)
- UI functions → return 0 (headless) or show ImGui dialog

### Phase 2: Device Detection & Init (Windows + Kinect SDK)
Implement:
1. `XamNuiGetDeviceStatus` → query `INuiSensor::NuiStatus()`
2. `XamNuiIsDeviceReady` → `INuiSensor::NuiStatus() == S_OK`
3. Camera elevation: `XamNuiCameraElevationGetAngle` / `SetAngle`

### Phase 3: Skeleton Tracking (Core Kinect Functionality)
Implement:
1. Skeleton enable/disable (mapped to `NuiSkeletonTrackingEnable`)
2. Skeleton frame retrieval (mapped to `NuiSkeletonGetNextFrame`)
3. Xbox 360 skeleton data format translation
4. `XamNuiSkeletonGetBestSkeletonIndex`
5. `XamNuiHudGetEngagedTrackingID` / `SetEngagedTrackingID`

### Phase 4: Advanced Features
1. Chat mic / audio (`XamNuiEnableChatMic`, `XamNuiIsChatMicEnabled`)
2. Identity/enrollment (`XamNuiIdentity*`, `XamUserNui*`)
3. Camera calibration data (`XamNuiGetDepthCalibration`, `XamNuiGetCameraIntrinsics`)
4. HUD integration (`XamNuiHud*`)

## Xbox 360 Skeleton Data Format

The Xbox 360 skeleton frame is big-endian and likely has this structure (to be confirmed
via game binary analysis):

```
X_NUI_SKELETON_FRAME {
  be<uint64_t> timestamp;
  be<uint32_t> frame_number;
  be<uint32_t> flags;
  be<float>[4] floor_clip_plane;    // xyzw
  be<float>[4] normal_to_gravity;   // xyzw
  X_NUI_SKELETON_DATA[6] skeletons;
}

X_NUI_SKELETON_DATA {
  be<uint32_t> tracking_state;      // 0=not tracked, 1=pos only, 2=tracked
  be<uint32_t> tracking_id;
  be<uint32_t> enrollment_index;
  be<uint32_t> user_index;
  be<float>[4] position;            // hip center xyzw
  be<float>[4][20] joint_positions; // 20 joints × xyzw
  be<uint32_t>[20] joint_states;    // tracking state per joint
  be<uint32_t> quality_flags;
}
```

## Device Status Structure (Xbox 360)

```cpp
struct X_NUI_DEVICE_STATUS {
  xe::be<uint32_t> unk0;
  xe::be<uint32_t> unk1;
  xe::be<uint32_t> unk2;
  xe::be<uint32_t> status;    // 0 = not connected, non-zero = connected
  xe::be<uint32_t> unk4;
  xe::be<uint32_t> unk5;
};
// Size: 24 bytes (confirmed via static_assert in existing code)
```

## Windows Kinect SDK Integration Strategy

### Detection
```cpp
// At startup, try to load Kinect10.dll
HMODULE kinect_dll = LoadLibraryW(L"Kinect10.dll");
if (kinect_dll) {
  // Bind function pointers
  // Call NuiGetSensorCount to check for sensors
}
```

### Sensor Lifecycle
```cpp
// Global state (one sensor supported for now)
static INuiSensor* g_nui_sensor = nullptr;
static bool g_nui_initialized = false;

void NuiShutdown() {
  if (g_nui_sensor) {
    g_nui_sensor->NuiShutdown();
    g_nui_sensor->Release();
    g_nui_sensor = nullptr;
  }
}
```

### Platform Guards
All Kinect SDK integration should be inside `#if XE_PLATFORM_WIN32` guards.

## Function Groups to Implement

### Group 1: Device Status (Priority: CRITICAL)
- `XamNuiGetDeviceStatus` – return status struct
- `XamNuiIsDeviceReady` – bool: is device usable?
- `XamKinectGetHardwareType` – what hardware revision?
- `XamNuiGetDeviceSerialNumber` – unique ID string

### Group 2: Camera Elevation (Priority: HIGH)
- `XamNuiCameraElevationSetAngle` – set tilt (-27 to +27 degrees)
- `XamNuiCameraElevationGetAngle` – get current tilt
- `XamNuiCameraElevationAutoTilt` – auto-tilt to find people
- `XamNuiCameraElevationStopMovement` – stop tilt motor
- `XamNuiCameraElevationReverseAutoTilt` – reverse auto-tilt
- `XamNuiCameraElevationSetCallback` – callback when tilt completes
- `XamNuiCameraTiltGetStatus` – get tilt motor status
- `XamNuiCameraTiltReportStatus` – report tilt status
- `XamNuiCameraAdjustTilt` – adjust by relative amount
- `XamNuiCameraTiltSetCallback` – set tilt callback
- `XamNuiCameraGetTiltControllerType` – controller type enum

### Group 3: Skeleton Tracking (Priority: HIGH)
- `XamNuiSkeletonGetBestSkeletonIndex` – which skeleton slot is "best" for HUD
- `XamNuiSkeletonScoreUpdate` – update skeleton scoring
- `XamNuiHudGetEngagedTrackingID` – get currently engaged player tracking ID
- `XamNuiHudSetEngagedTrackingID` – set engaged tracking ID
- `XamNuiHudGetEngagedEnrollmentIndex` – enrollment index of engaged user
- `XamNuiPlayerEngagementUpdate` – update player engagement state

### Group 4: Misc Device/System (Priority: MEDIUM)
- `XamNuiSetForceDeviceOff` – force device off
- `XamNuiEnableChatMic` – enable microphone for chat
- `XamNuiIsChatMicEnabled` – is chat mic enabled?
- `XamNuiGetFanRate` – fan speed
- `XamNuiGetSupportString` – support string for troubleshooting
- `XamNuiGetCameraIntrinsics` – lens calibration data
- `XamNuiGetDepthCalibration` – depth calibration data
- `XamNuiGetLoadedDepthCalibration` – get currently loaded calibration
- `XamNuiStoreDepthCalibration` – persist calibration
- `XamNuiGetTrueColorInfo` – color stream info
- `XamNuiCameraSetFlags` – set camera operational flags
- `XamNuiCameraRememberFloor` – calibrate floor plane
- `XamNuiNatalCameraUpdateStarting` / `XamNuiNatalCameraUpdateComplete` – firmware update hooks
- `XamNuiHudGetVersions` – get HUD version info
- `XamNuiHudGetInitializeFlags` – initialization flags
- `XamNuiHudIsEnabled` – is HUD enabled?
- `XamNuiHudInterpretFrame` – process skeleton frame for HUD input
- `XamNuiHudEnableInputFilter` – enable/disable HUD input filtering
- `XamNuiGetSystemGestureControl` – get system gesture control interface

### Group 5: User Identity & Enrollment (Priority: LOW for basic gaming)
- `XamNuiIdentityGetEnrollmentInfo` – enrollment info for a tracking slot
- `XamNuiIdentityUnenroll` – unenroll a user
- `XamNuiIdentityGetColorTexture` – color image of identified user
- `XamNuiIdentityGetQualityFlags` – quality of identity tracking
- `XamNuiIdentityGetQualityFlagsMessage` – human-readable quality message
- `XamNuiIdentityEnrollForSignIn` – start face enrollment
- `XamNuiIdentityGetSessionId` – session ID for identity
- `XamNuiIdentityIdentifyWithBiometric` – trigger biometric identification
- `XamNuiIdentityAbort` – abort identity operation
- `XamUserNuiIsBiometricEnabled` – is biometric enabled for user?
- `XamUserNuiEnableBiometric` – enable biometric
- `XamUserNuiBind` – bind NUI to user slot
- `XamUserNuiUnbind` – unbind NUI from user slot
- `XamUserNuiGetUserIndex` – get user index for tracked skeleton
- `XamUserNuiGetEnrollmentIndex` – get enrollment index for user
- `XamUserNuiGetUserIndexForBind` – user index that can be bound
- `XamUserNuiGetUserIndexForSignin` – user index for sign-in

### Group 6: UI Functions (Priority: LOW – game UI, can stub or show dialogs)
Many `XamShowNui*UI` functions simply display Kinect-related UI overlays. They can all
be stubbed to return 0 (headless) or show an ImGui message box.

### Group 7: Automation & Control (Priority: LOW)
- `XamEnableNuiAutomation` – enable NUI automation mode
- `XamIsNuiAutomationEnabled` – is automation enabled?
- `XamIsNuiUIActive` – is NUI UI currently showing?
- `XamLoaderIsKinectUIPreferredForLogonTitle` – should Kinect UI be used for sign-in?
- `XamReportKinectSettingsChangedEvent` – notify that settings changed
- `ControlpackNuiCursorSetTrackingId` – set cursor tracking ID (controller pack)
- `ControlPackSideNavControlGetNuiHandle` – get NUI handle for side nav
