# Xenia NUI/Kinect – Current Implementation Notes

## File Under Investigation
`src/xenia/kernel/xam/xam_nui.cc`

## Current State (as of research phase)

The file currently contains only **two implemented/stubbed exports**:

### 1. `XamNuiGetDeviceStatus` (ordinal 0x885)
```cpp
struct X_NUI_DEVICE_STATUS {
  xe::be<uint32_t> unk0;
  xe::be<uint32_t> unk1;
  xe::be<uint32_t> unk2;
  xe::be<uint32_t> status;  // offset 12 – 0 = not connected
  xe::be<uint32_t> unk4;
  xe::be<uint32_t> unk5;
};
// Size: 24 bytes
```
Currently zeros the structure and sets `status = 0` (not connected). Pure stub.

### 2. `XamShowNuiTroubleshooterUI` (ordinal 0x46B)
Shows an ImGui dialog box telling the user there is a Kinect problem.
Mostly functional for its purpose (display a UI message).

## All NUI/Kinect Exports in xam_table.inc

There are **64+ NUI-related exports** in `xam_table.inc`. Below is the complete list:

| Ordinal  | Function Name |
|----------|---------------|
| 0x040C   | XamIsNuiUIActive |
| 0x0356   | XamShowNuiGuideUI |
| 0x046B   | XamShowNuiTroubleshooterUI *(stubbed)* |
| 0x046C   | XamTestShowNuiTroubleshooterUI |
| 0x046E   | XamShowNuiAchievementsUI |
| 0x046F   | XamShowNuiCommunitySessionsUI |
| 0x0470   | XamShowNuiDeviceSelectorUI |
| 0x0471   | XamShowNuiDirtyDiscErrorUI |
| 0x0472   | XamShowNuiFriendsUI |
| 0x0473   | XamShowNuiGameInviteUI |
| 0x0474   | XamShowNuiGamerCardUIForXUID |
| 0x0475   | XamShowNuiMarketplaceDownloadItemsUI |
| 0x0476   | XamShowNuiMarketplaceUI |
| 0x0477   | XamShowNuiMessageBoxUI |
| 0x0478   | XamShowNuiPartyUI |
| 0x0479   | XamShowNuiFriendRequestUI |
| 0x047A   | XamShowNuiSigninUI |
| 0x047B   | XamShowNuiControllerRequiredUI |
| 0x047C   | XamShowNuiGamesUI |
| 0x047D   | XamShowNuiHardwareRequiredUI |
| 0x04AF   | XamShowNuiMessagesUI |
| 0x04D3   | XamShowNuiJoinSessionInProgressUI |
| 0x04F2   | XamShowNuiVideoRichPresenceUI |
| 0x051B   | XamNuiHudGetEngagedEnrollmentIndex |
| 0x083A   | XamNuiHudGetEngagedTrackingID |
| 0x083C   | XamNuiHudSetEngagedTrackingID |
| 0x0840   | XamLoaderIsKinectUIPreferredForLogonTitle |
| 0x085A   | XamNuiIdentityGetEnrollmentInfo |
| 0x085B   | XamNuiIdentityUnenroll |
| 0x085C   | XamNuiIdentityGetColorTexture |
| 0x085D   | XamReportKinectSettingsChangedEvent |
| 0x085E   | XamNuiEnableChatMic |
| 0x085F   | XamNuiIsChatMicEnabled |
| 0x0860   | XamNuiGetDepthCalibration |
| 0x0861   | XamNuiStoreDepthCalibration |
| 0x0862   | XamUserNuiIsBiometricEnabled |
| 0x0863   | XamUserNuiEnableBiometric |
| 0x0864   | XamNuiCameraSetFlags |
| 0x0865   | XamNuiCameraRememberFloor |
| 0x0866   | XamUserNuiGetUserIndexForBind |
| 0x0867   | XamUserNuiGetUserIndexForSignin |
| 0x0868   | XamUserNuiBind |
| 0x0869   | XamUserNuiGetUserIndex |
| 0x086A   | XamUserNuiGetEnrollmentIndex |
| 0x086B   | XamUserNuiUnbind |
| 0x086C   | XamNuiIdentityGetQualityFlagsMessage |
| 0x086D   | XamNuiCameraTiltSetCallback |
| 0x086E   | XamNuiHudIsEnabled |
| 0x086F   | XamNuiCameraGetTiltControllerType |
| 0x0870   | XamNuiCameraTiltGetStatus |
| 0x0871   | XamNuiIdentityGetQualityFlags |
| 0x0872   | XamNuiIdentityEnrollForSignIn |
| 0x0873   | XamNuiIdentityGetSessionId |
| 0x0874   | XamNuiIdentityIdentifyWithBiometric |
| 0x0876   | XamEnableNuiAutomation |
| 0x0877   | XamNuiGetSystemGestureControl |
| 0x087A   | XamIsNuiAutomationEnabled |
| 0x087C   | XamNuiHudInterpretFrame |
| 0x087D   | XamNuiHudEnableInputFilter |
| 0x087E   | ControlpackNuiCursorSetTrackingId |
| 0x087F   | XamNuiHudGetInitializeFlags |
| 0x0880   | XamNuiCameraElevationSetAngle |
| 0x0881   | XamNuiCameraElevationGetAngle |
| 0x0882   | XamNuiCameraElevationAutoTilt |
| 0x0883   | XamNuiCameraElevationStopMovement |
| 0x0884   | XamNuiCameraElevationSetCallback |
| 0x0885   | XamNuiGetDeviceStatus *(stubbed)* |
| 0x0886   | XamNuiNatalCameraUpdateStarting |
| 0x0887   | XamNuiNatalCameraUpdateComplete |
| 0x0888   | XamNuiHudGetVersions |
| 0x088C   | XamNuiCameraElevationReverseAutoTilt |
| 0x088E   | ControlPackSideNavControlGetNuiHandle |
| 0x0890   | XamNuiIsDeviceReady |
| 0x0891   | XamNuiSetForceDeviceOff |
| 0x0892   | XamNuiPlayerEngagementUpdate |
| 0x0893   | XamNuiSkeletonGetBestSkeletonIndex |
| 0x0894   | XamNuiSkeletonScoreUpdate |
| 0x0895   | XamNuiCameraTiltReportStatus |
| 0x0896   | XamNuiCameraAdjustTilt |
| 0x0897   | XamNuiIdentityAbort |
| 0x08DD   | XamNuiGetDeviceSerialNumber |
| 0x08DE   | XamNuiGetLoadedDepthCalibration |
| 0x08DF   | XamNuiGetTrueColorInfo |
| 0x08E0   | XamNuiGetCameraIntrinsics |
| 0x08E1   | XamNuiGetFanRate |
| 0x08E2   | XamNuiGetSupportString |
| 0x08E3   | XamKinectGetHardwareType |

## How Xenia Kernel Exports Work

1. `xam_table.inc` declares all exports with ordinals.
2. Each export has a corresponding `_entry` function in a `.cc` file.
3. Exports are declared with `DECLARE_XAM_EXPORT1(Name, category, tag)`.
4. The `DECLARE_XAM_EMPTY_REGISTER_EXPORTS(NUI)` at the bottom of `xam_nui.cc` registers the group.
5. Category is something like `kNone`, `kInput`, etc.
6. Tag is `kStub` (not implemented), `kImplemented`, or `kSketchy`.

## Shim Utilities Relevant to NUI Implementation

From `src/xenia/kernel/util/shim_utils.h`:
- `dword_t` – 32-bit guest integer
- `pointer_t<T>` – typed guest pointer
- `lpdword_t` – pointer to 32-bit value
- `dword_result_t` – 32-bit return value
- `unknown_t` – unknown argument type
- `lpvoid_t` – void pointer

## Notes on Implementation Strategy

For a "no Kinect connected" emulation (baseline), all device-status functions should
return appropriate "not connected" / "device not found" error codes.

For actual Kinect SDK 1.8 integration, the following functions map to Windows SDK calls
(see `docs/kinect/02_sdk_mapping.md`).
