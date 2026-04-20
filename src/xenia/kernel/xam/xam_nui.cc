/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2022 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#include "xenia/base/logging.h"
#include "xenia/emulator.h"
#include "xenia/kernel/kernel_flags.h"
#include "xenia/kernel/kernel_state.h"
#include "xenia/kernel/util/shim_utils.h"
#include "xenia/kernel/xam/xam_private.h"
#include "xenia/ui/imgui_dialog.h"
#include "xenia/ui/imgui_drawer.h"
#include "xenia/ui/window.h"
#include "xenia/ui/windowed_app_context.h"
#include "xenia/xbox.h"

namespace xe {
namespace kernel {
namespace xam {

extern std::atomic<int> xam_dialogs_shown_;

// NUI device status structure (24 bytes, big-endian fields).
// status == 0 means not connected.
struct X_NUI_DEVICE_STATUS {
  xe::be<uint32_t> unk0;
  xe::be<uint32_t> unk1;
  xe::be<uint32_t> unk2;
  xe::be<uint32_t> status;
  xe::be<uint32_t> unk4;
  xe::be<uint32_t> unk5;
};
static_assert(sizeof(X_NUI_DEVICE_STATUS) == 24, "Size matters");

// Enrollment index returned when no identity is associated with a skeleton.
static constexpr uint32_t kNuiEnrollmentIndexUnknown = 0xFFFFFFFF;

// ============================================================================
// Device status / readiness
// ============================================================================

void XamNuiGetDeviceStatus_entry(pointer_t<X_NUI_DEVICE_STATUS> status_ptr) {
  status_ptr.Zero();
  status_ptr->status = 0;  // Not connected.
}
DECLARE_XAM_EXPORT1(XamNuiGetDeviceStatus, kNone, kStub);

// Returns non-zero when the Kinect sensor is connected and ready.
dword_result_t XamNuiIsDeviceReady_entry() {
  // No sensor connected.
  return 0;
}
DECLARE_XAM_EXPORT1(XamNuiIsDeviceReady, kNone, kStub);

// Returns a hardware type identifier for the connected Kinect sensor.
// 0 = no hardware / unknown.
dword_result_t XamKinectGetHardwareType_entry() { return 0; }
DECLARE_XAM_EXPORT1(XamKinectGetHardwareType, kNone, kStub);

// Fills a buffer with the sensor's USB serial number string.
dword_result_t XamNuiGetDeviceSerialNumber_entry(unknown_t buf,
                                                 unknown_t buf_len) {
  return X_ERROR_DEVICE_NOT_CONNECTED;
}
DECLARE_XAM_EXPORT1(XamNuiGetDeviceSerialNumber, kNone, kStub);

// Returns a support/diagnostic string for the current device state.
dword_result_t XamNuiGetSupportString_entry(unknown_t buf, unknown_t buf_len) {
  return 0;
}
DECLARE_XAM_EXPORT1(XamNuiGetSupportString, kNone, kStub);

// Forces the Kinect sensor off (power management).
dword_result_t XamNuiSetForceDeviceOff_entry(dword_t force_off) { return 0; }
DECLARE_XAM_EXPORT1(XamNuiSetForceDeviceOff, kNone, kStub);

// ============================================================================
// Camera elevation / tilt
// ============================================================================

// Sets the Kinect camera tilt angle in degrees (-27 to +27).
dword_result_t XamNuiCameraElevationSetAngle_entry(dword_t angle_degrees) {
  return X_ERROR_DEVICE_NOT_CONNECTED;
}
DECLARE_XAM_EXPORT1(XamNuiCameraElevationSetAngle, kNone, kStub);

// Returns the current camera tilt angle and motor movement flags.
dword_result_t XamNuiCameraElevationGetAngle_entry(lpdword_t angle_out,
                                                   lpdword_t moving_flags_out) {
  if (angle_out) {
    *angle_out = 0;
  }
  if (moving_flags_out) {
    *moving_flags_out = 0;
  }
  return X_ERROR_DEVICE_NOT_CONNECTED;
}
DECLARE_XAM_EXPORT1(XamNuiCameraElevationGetAngle, kNone, kStub);

// Starts auto-tilt so the camera automatically frames detected players.
dword_result_t XamNuiCameraElevationAutoTilt_entry(unknown_t unk1,
                                                   unknown_t unk2) {
  return X_ERROR_DEVICE_NOT_CONNECTED;
}
DECLARE_XAM_EXPORT1(XamNuiCameraElevationAutoTilt, kNone, kStub);

// Stops the tilt motor as quickly as possible.
dword_result_t XamNuiCameraElevationStopMovement_entry() {
  return X_ERROR_DEVICE_NOT_CONNECTED;
}
DECLARE_XAM_EXPORT1(XamNuiCameraElevationStopMovement, kNone, kStub);

// Reverses an ongoing auto-tilt operation.
dword_result_t XamNuiCameraElevationReverseAutoTilt_entry(unknown_t unk1,
                                                          unknown_t unk2) {
  return X_ERROR_DEVICE_NOT_CONNECTED;
}
DECLARE_XAM_EXPORT1(XamNuiCameraElevationReverseAutoTilt, kNone, kStub);

// Registers a callback invoked when a tilt operation completes.
dword_result_t XamNuiCameraElevationSetCallback_entry(unknown_t callback,
                                                      unknown_t context) {
  return 0;
}
DECLARE_XAM_EXPORT1(XamNuiCameraElevationSetCallback, kNone, kStub);

// Adjusts the camera angle based on depth-map player detection
// (NuiCameraAdjustTilt equivalent).
dword_result_t XamNuiCameraAdjustTilt_entry(unknown_t tilt_flags,
                                            unknown_t space_above_head,
                                            unknown_t far_space,
                                            unknown_t preferred_dist,
                                            unknown_t tilt_objects_ptr,
                                            unknown_t overlapped_ptr) {
  return X_ERROR_DEVICE_NOT_CONNECTED;
}
DECLARE_XAM_EXPORT1(XamNuiCameraAdjustTilt, kNone, kStub);

// Returns the type identifier of the current tilt controller.
dword_result_t XamNuiCameraGetTiltControllerType_entry() { return 0; }
DECLARE_XAM_EXPORT1(XamNuiCameraGetTiltControllerType, kNone, kStub);

// Returns the current status of the tilt motor.
dword_result_t XamNuiCameraTiltGetStatus_entry(unknown_t status_out) {
  return 0;
}
DECLARE_XAM_EXPORT1(XamNuiCameraTiltGetStatus, kNone, kStub);

// Reports tilt motor status (internal telemetry).
dword_result_t XamNuiCameraTiltReportStatus_entry(unknown_t unk1,
                                                  unknown_t unk2) {
  return 0;
}
DECLARE_XAM_EXPORT1(XamNuiCameraTiltReportStatus, kNone, kStub);

// Registers a callback invoked on tilt-status changes.
dword_result_t XamNuiCameraTiltSetCallback_entry(unknown_t callback,
                                                 unknown_t context) {
  return 0;
}
DECLARE_XAM_EXPORT1(XamNuiCameraTiltSetCallback, kNone, kStub);

// Sets operational flags on the camera sensor.
dword_result_t XamNuiCameraSetFlags_entry(unknown_t flags) { return 0; }
DECLARE_XAM_EXPORT1(XamNuiCameraSetFlags, kNone, kStub);

// Persists the current floor plane for subsequent sessions.
dword_result_t XamNuiCameraRememberFloor_entry(unknown_t unk1) { return 0; }
DECLARE_XAM_EXPORT1(XamNuiCameraRememberFloor, kNone, kStub);

// ============================================================================
// Camera intrinsics / calibration
// ============================================================================

// Fills a buffer with lens intrinsic calibration parameters.
dword_result_t XamNuiGetCameraIntrinsics_entry(unknown_t type,
                                               unknown_t out_buf) {
  return X_ERROR_DEVICE_NOT_CONNECTED;
}
DECLARE_XAM_EXPORT1(XamNuiGetCameraIntrinsics, kNone, kStub);

// Fills a buffer with depth calibration data.
dword_result_t XamNuiGetDepthCalibration_entry(unknown_t out_buf,
                                               unknown_t buf_len) {
  return X_ERROR_DEVICE_NOT_CONNECTED;
}
DECLARE_XAM_EXPORT1(XamNuiGetDepthCalibration, kNone, kStub);

// Returns depth calibration data that was previously loaded into the system.
dword_result_t XamNuiGetLoadedDepthCalibration_entry(unknown_t out_buf,
                                                     unknown_t buf_len) {
  return X_ERROR_DEVICE_NOT_CONNECTED;
}
DECLARE_XAM_EXPORT1(XamNuiGetLoadedDepthCalibration, kNone, kStub);

// Persists depth calibration data to non-volatile storage.
dword_result_t XamNuiStoreDepthCalibration_entry(unknown_t in_buf,
                                                 unknown_t buf_len) {
  return X_ERROR_DEVICE_NOT_CONNECTED;
}
DECLARE_XAM_EXPORT1(XamNuiStoreDepthCalibration, kNone, kStub);

// Returns color camera stream metadata (format, resolution, etc.).
dword_result_t XamNuiGetTrueColorInfo_entry(unknown_t out_buf) {
  return X_ERROR_DEVICE_NOT_CONNECTED;
}
DECLARE_XAM_EXPORT1(XamNuiGetTrueColorInfo, kNone, kStub);

// Returns the Kinect fan speed.
dword_result_t XamNuiGetFanRate_entry(unknown_t out_rate) { return 0; }
DECLARE_XAM_EXPORT1(XamNuiGetFanRate, kNone, kStub);

// ============================================================================
// Audio / chat mic
// ============================================================================

// Enables or disables the Kinect microphone array for chat.
dword_result_t XamNuiEnableChatMic_entry(dword_t enable) { return 0; }
DECLARE_XAM_EXPORT1(XamNuiEnableChatMic, kNone, kStub);

// Returns non-zero if the chat mic is currently enabled.
dword_result_t XamNuiIsChatMicEnabled_entry() { return 0; }
DECLARE_XAM_EXPORT1(XamNuiIsChatMicEnabled, kNone, kStub);

// Runs acoustic calibration for the microphone array.
dword_result_t XamNatalDeviceAudioCalibrate_entry(unknown_t unk1,
                                                  unknown_t unk2) {
  return 0;
}
DECLARE_XAM_EXPORT1(XamNatalDeviceAudioCalibrate, kNone, kStub);

// ============================================================================
// Skeleton HUD / engagement tracking
// ============================================================================

// Returns the index of the skeleton best suited for HUD engagement.
// Writes 0xFFFFFFFF (no skeleton) when no device is connected.
dword_result_t XamNuiSkeletonGetBestSkeletonIndex_entry(
    unknown_t skeleton_frame_ptr, lpdword_t index_out) {
  if (index_out) {
    *index_out = 0xFFFFFFFF;
  }
  return 0;
}
DECLARE_XAM_EXPORT1(XamNuiSkeletonGetBestSkeletonIndex, kNone, kStub);

// Updates the internal skeleton scoring used to pick the best skeleton.
dword_result_t XamNuiSkeletonScoreUpdate_entry(unknown_t skeleton_frame_ptr,
                                               unknown_t score_buf) {
  return 0;
}
DECLARE_XAM_EXPORT1(XamNuiSkeletonScoreUpdate, kNone, kStub);

// Returns the tracking ID of the currently engaged (focused) player.
dword_result_t XamNuiHudGetEngagedTrackingID_entry(lpdword_t tracking_id_out) {
  if (tracking_id_out) {
    *tracking_id_out = 0;
  }
  return 0;
}
DECLARE_XAM_EXPORT1(XamNuiHudGetEngagedTrackingID, kNone, kStub);

// Sets the tracking ID of the player the HUD should focus on.
dword_result_t XamNuiHudSetEngagedTrackingID_entry(dword_t tracking_id) {
  return 0;
}
DECLARE_XAM_EXPORT1(XamNuiHudSetEngagedTrackingID, kNone, kStub);

// Returns the enrollment index associated with the currently engaged player.
// Returns kNuiEnrollmentIndexUnknown when not enrolled or no device.
dword_result_t XamNuiHudGetEngagedEnrollmentIndex_entry() {
  return kNuiEnrollmentIndexUnknown;
}
DECLARE_XAM_EXPORT1(XamNuiHudGetEngagedEnrollmentIndex, kNone, kStub);

// Sends a player engagement update packet (0x1C bytes) to the NUI system.
dword_result_t XamNuiPlayerEngagementUpdate_entry(unknown_t data_ptr,
                                                  unknown_t data_len) {
  return 0;
}
DECLARE_XAM_EXPORT1(XamNuiPlayerEngagementUpdate, kNone, kStub);

// ============================================================================
// HUD system
// ============================================================================

// Returns non-zero if the Kinect HUD overlay is currently active.
dword_result_t XamNuiHudIsEnabled_entry() { return 0; }
DECLARE_XAM_EXPORT1(XamNuiHudIsEnabled, kNone, kStub);

// Processes a skeleton frame to extract HUD input (cursor position, gestures).
dword_result_t XamNuiHudInterpretFrame_entry(unknown_t frame_ptr,
                                             unknown_t out_state_ptr) {
  return 0;
}
DECLARE_XAM_EXPORT1(XamNuiHudInterpretFrame, kNone, kStub);

// Enables or disables HUD input filtering.
dword_result_t XamNuiHudEnableInputFilter_entry(dword_t enable) { return 0; }
DECLARE_XAM_EXPORT1(XamNuiHudEnableInputFilter, kNone, kStub);

// Returns version information for the HUD subsystem.
dword_result_t XamNuiHudGetVersions_entry(unknown_t out_buf) { return 0; }
DECLARE_XAM_EXPORT1(XamNuiHudGetVersions, kNone, kStub);

// Returns the initialization flags the HUD was started with.
dword_result_t XamNuiHudGetInitializeFlags_entry() { return 0; }
DECLARE_XAM_EXPORT1(XamNuiHudGetInitializeFlags, kNone, kStub);

// Returns a pointer to the system gesture control interface.
dword_result_t XamNuiGetSystemGestureControl_entry() { return 0; }
DECLARE_XAM_EXPORT1(XamNuiGetSystemGestureControl, kNone, kStub);

// Sets the NUI cursor tracking ID for a controller-pack cursor.
dword_result_t ControlpackNuiCursorSetTrackingId_entry(dword_t tracking_id) {
  return 0;
}
DECLARE_XAM_EXPORT1(ControlpackNuiCursorSetTrackingId, kNone, kStub);

// Returns the NUI handle for a side-nav control-pack element.
dword_result_t ControlPackSideNavControlGetNuiHandle_entry(unknown_t unk1) {
  return 0;
}
DECLARE_XAM_EXPORT1(ControlPackSideNavControlGetNuiHandle, kNone, kStub);

// ============================================================================
// Identity / enrollment (biometric sign-in) – Xbox 360 specific, no hardware
// ============================================================================

// Aborts all pending identity operations.
dword_result_t XamNuiIdentityAbort_entry() { return X_E_FAIL; }
DECLARE_XAM_EXPORT1(XamNuiIdentityAbort, kNone, kStub);

// Starts biometric enrollment for sign-in for a tracked skeleton.
dword_result_t XamNuiIdentityEnrollForSignIn_entry(unknown_t tracking_id,
                                                   unknown_t flags,
                                                   unknown_t callback,
                                                   unknown_t context) {
  return X_E_FAIL;
}
DECLARE_XAM_EXPORT1(XamNuiIdentityEnrollForSignIn, kNone, kStub);

// Removes an enrollment entry.
dword_result_t XamNuiIdentityUnenroll_entry(dword_t enrollment_index) {
  return X_E_FAIL;
}
DECLARE_XAM_EXPORT1(XamNuiIdentityUnenroll, kNone, kStub);

// Returns a color texture snapshot of the identified user.
dword_result_t XamNuiIdentityGetColorTexture_entry(unknown_t enrollment_index,
                                                   unknown_t texture_out) {
  return X_E_FAIL;
}
DECLARE_XAM_EXPORT1(XamNuiIdentityGetColorTexture, kNone, kStub);

// Returns enrollment information for a tracked skeleton slot.
dword_result_t XamNuiIdentityGetEnrollmentInfo_entry(unknown_t tracking_id,
                                                     unknown_t info_out) {
  return X_E_FAIL;
}
DECLARE_XAM_EXPORT1(XamNuiIdentityGetEnrollmentInfo, kNone, kStub);

// Returns quality flags describing the current identity tracking quality.
dword_result_t XamNuiIdentityGetQualityFlags_entry(unknown_t tracking_id,
                                                   unknown_t flags_out) {
  return X_E_FAIL;
}
DECLARE_XAM_EXPORT1(XamNuiIdentityGetQualityFlags, kNone, kStub);

// Fills a buffer with a human-readable quality flags description string.
dword_result_t XamNuiIdentityGetQualityFlagsMessage_entry(unknown_t flags,
                                                          unknown_t buf,
                                                          unknown_t buf_len) {
  return 0;
}
DECLARE_XAM_EXPORT1(XamNuiIdentityGetQualityFlagsMessage, kNone, kStub);

// Returns a session identifier for the current identity session.
dword_result_t XamNuiIdentityGetSessionId_entry(unknown_t session_id_out) {
  return 0;
}
DECLARE_XAM_EXPORT1(XamNuiIdentityGetSessionId, kNone, kStub);

// Triggers biometric identification for a skeleton's face.
dword_result_t XamNuiIdentityIdentifyWithBiometric_entry(unknown_t tracking_id,
                                                         unknown_t flags,
                                                         unknown_t callback,
                                                         unknown_t context) {
  return X_E_FAIL;
}
DECLARE_XAM_EXPORT1(XamNuiIdentityIdentifyWithBiometric, kNone, kStub);

// ============================================================================
// Per-user NUI binding / biometric
// ============================================================================

// Returns non-zero if biometric (face recognition) is enabled for the user.
dword_result_t XamUserNuiIsBiometricEnabled_entry(dword_t user_index) {
  return 0;
}
DECLARE_XAM_EXPORT1(XamUserNuiIsBiometricEnabled, kNone, kStub);

// Enables or disables biometric identification for a user slot.
dword_result_t XamUserNuiEnableBiometric_entry(dword_t user_index,
                                               dword_t enable) {
  return 0;
}
DECLARE_XAM_EXPORT1(XamUserNuiEnableBiometric, kNone, kStub);

// Binds the Kinect sensor to a user slot.
dword_result_t XamUserNuiBind_entry(dword_t user_index) { return 0; }
DECLARE_XAM_EXPORT1(XamUserNuiBind, kNone, kStub);

// Removes the Kinect binding for a user slot.
dword_result_t XamUserNuiUnbind_entry(dword_t user_index) { return 0; }
DECLARE_XAM_EXPORT1(XamUserNuiUnbind, kNone, kStub);

// Returns the user-slot index associated with the currently tracked skeleton.
// Returns 0xFFFFFFFF when not bound to any user.
dword_result_t XamUserNuiGetUserIndex_entry(unknown_t tracking_id,
                                            lpdword_t user_index_out) {
  if (user_index_out) {
    *user_index_out = 0xFFFFFFFF;
  }
  return 0;
}
DECLARE_XAM_EXPORT1(XamUserNuiGetUserIndex, kNone, kStub);

// Returns the enrollment index for a bound user.
dword_result_t XamUserNuiGetEnrollmentIndex_entry(dword_t user_index,
                                                  lpdword_t enrollment_out) {
  if (enrollment_out) {
    *enrollment_out = kNuiEnrollmentIndexUnknown;
  }
  return 0;
}
DECLARE_XAM_EXPORT1(XamUserNuiGetEnrollmentIndex, kNone, kStub);

// Returns the user-slot index that is eligible for NUI binding.
dword_result_t XamUserNuiGetUserIndexForBind_entry(lpdword_t user_index_out) {
  if (user_index_out) {
    *user_index_out = 0;
  }
  return 0;
}
DECLARE_XAM_EXPORT1(XamUserNuiGetUserIndexForBind, kNone, kStub);

// Returns the user-slot index that the sign-in flow should use.
dword_result_t XamUserNuiGetUserIndexForSignin_entry(
    lpdword_t user_index_out) {
  if (user_index_out) {
    *user_index_out = 0;
  }
  return 0;
}
DECLARE_XAM_EXPORT1(XamUserNuiGetUserIndexForSignin, kNone, kStub);

// ============================================================================
// Firmware update lifecycle hooks
// ============================================================================

// Called by the system before a Kinect firmware update begins.
dword_result_t XamNuiNatalCameraUpdateStarting_entry(unknown_t unk1) {
  return 0;
}
DECLARE_XAM_EXPORT1(XamNuiNatalCameraUpdateStarting, kNone, kStub);

// Called by the system after a Kinect firmware update completes.
dword_result_t XamNuiNatalCameraUpdateComplete_entry(unknown_t unk1) {
  return 0;
}
DECLARE_XAM_EXPORT1(XamNuiNatalCameraUpdateComplete, kNone, kStub);

// ============================================================================
// Automation / playback
// ============================================================================

dword_result_t XamEnableNuiAutomation_entry(dword_t enable) { return 0; }
DECLARE_XAM_EXPORT1(XamEnableNuiAutomation, kNone, kStub);

dword_result_t XamIsNuiAutomationEnabled_entry() { return 0; }
DECLARE_XAM_EXPORT1(XamIsNuiAutomationEnabled, kNone, kStub);

dword_result_t XamEnableNatalPlayback_entry(dword_t enable) { return 0; }
DECLARE_XAM_EXPORT1(XamEnableNatalPlayback, kNone, kStub);

dword_result_t XamIsNatalPlaybackEnabled_entry() { return 0; }
DECLARE_XAM_EXPORT1(XamIsNatalPlaybackEnabled, kNone, kStub);

// ============================================================================
// Misc system / settings
// ============================================================================

// Returns non-zero if a Kinect UI overlay is currently showing.
dword_result_t XamIsNuiUIActive_entry() { return 0; }
DECLARE_XAM_EXPORT1(XamIsNuiUIActive, kNone, kStub);

// Returns non-zero if the title's sign-in flow should prefer Kinect UI.
dword_result_t XamLoaderIsKinectUIPreferredForLogonTitle_entry() { return 0; }
DECLARE_XAM_EXPORT1(XamLoaderIsKinectUIPreferredForLogonTitle, kNone, kStub);

// Notifies the system that Kinect-related settings have changed.
dword_result_t XamReportKinectSettingsChangedEvent_entry(unknown_t unk1) {
  return 0;
}
DECLARE_XAM_EXPORT1(XamReportKinectSettingsChangedEvent, kNone, kStub);

// ============================================================================
// NUI guide / UI overlays
// ============================================================================

dword_result_t XamShowNuiGuideUI_entry(unknown_t unk1, unknown_t unk2,
                                       unknown_t unk3) {
  return 0;
}
DECLARE_XAM_EXPORT1(XamShowNuiGuideUI, kNone, kStub);

dword_result_t XamTestShowNuiTroubleshooterUI_entry(unknown_t unk1,
                                                    unknown_t unk2,
                                                    unknown_t unk3) {
  return 0;
}
DECLARE_XAM_EXPORT1(XamTestShowNuiTroubleshooterUI, kNone, kStub);

dword_result_t XamShowNuiTroubleshooterUI_entry(unknown_t unk1, unknown_t unk2,
                                                unknown_t unk3) {
  // unk1 is 0xFF - possibly user index?
  // unk2, unk3 appear to always be zero.

  if (cvars::headless) {
    return 0;
  }

  const Emulator* emulator = kernel_state()->emulator();
  ui::Window* display_window = emulator->display_window();
  ui::ImGuiDrawer* imgui_drawer = emulator->imgui_drawer();
  if (display_window && imgui_drawer) {
    xe::threading::Fence fence;
    if (display_window->app_context().CallInUIThreadSynchronous([&]() {
          xe::ui::ImGuiDialog::ShowMessageBox(
              imgui_drawer, "NUI Troubleshooter",
              "The game has indicated there is a problem with NUI (Kinect).")
              ->Then(&fence);
        })) {
      ++xam_dialogs_shown_;
      fence.Wait();
      --xam_dialogs_shown_;
    }
  }

  return 0;
}
DECLARE_XAM_EXPORT1(XamShowNuiTroubleshooterUI, kNone, kStub);

// Remaining XamShowNui*UI overlays – all stubbed to return 0.
// Games use these for Kinect-specific marketplace/friends/party UI that has no
// equivalent in an emulated environment.
#define STUB_NUI_SHOW_UI(name)                                            \
  dword_result_t name##_entry(unknown_t unk1, unknown_t unk2,            \
                               unknown_t unk3) {                         \
    return 0;                                                             \
  }                                                                       \
  DECLARE_XAM_EXPORT1(name, kNone, kStub)

STUB_NUI_SHOW_UI(XamShowNuiAchievementsUI);
STUB_NUI_SHOW_UI(XamShowNuiCommunitySessionsUI);
STUB_NUI_SHOW_UI(XamShowNuiDeviceSelectorUI);
STUB_NUI_SHOW_UI(XamShowNuiDirtyDiscErrorUI);
STUB_NUI_SHOW_UI(XamShowNuiFriendsUI);
STUB_NUI_SHOW_UI(XamShowNuiGameInviteUI);
STUB_NUI_SHOW_UI(XamShowNuiGamerCardUIForXUID);
STUB_NUI_SHOW_UI(XamShowNuiMarketplaceDownloadItemsUI);
STUB_NUI_SHOW_UI(XamShowNuiMarketplaceUI);
STUB_NUI_SHOW_UI(XamShowNuiMessageBoxUI);
STUB_NUI_SHOW_UI(XamShowNuiPartyUI);
STUB_NUI_SHOW_UI(XamShowNuiFriendRequestUI);
STUB_NUI_SHOW_UI(XamShowNuiSigninUI);
STUB_NUI_SHOW_UI(XamShowNuiControllerRequiredUI);
STUB_NUI_SHOW_UI(XamShowNuiGamesUI);
STUB_NUI_SHOW_UI(XamShowNuiHardwareRequiredUI);
STUB_NUI_SHOW_UI(XamShowNuiMessagesUI);
STUB_NUI_SHOW_UI(XamShowNuiJoinSessionInProgressUI);
STUB_NUI_SHOW_UI(XamShowNuiVideoRichPresenceUI);

#undef STUB_NUI_SHOW_UI

}  // namespace xam
}  // namespace kernel
}  // namespace xe

DECLARE_XAM_EMPTY_REGISTER_EXPORTS(NUI);
