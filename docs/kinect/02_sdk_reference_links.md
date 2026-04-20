# Windows Kinect SDK 1.8 – Reference Links & API Documentation

## Official Microsoft Documentation

> **Note:** Many of these links may require the Wayback Machine / archive.org as Microsoft has
> removed the Kinect 1.x developer documentation from their live site.

### Primary Reference Pages

| Topic | URL (live) | Archive URL |
|-------|-----------|-------------|
| NUI API Reference (top-level) | https://learn.microsoft.com/en-us/previous-versions/windows/kinect-1.8/hh855347(v=ieb.10) | https://web.archive.org/web/20150905084124/https://msdn.microsoft.com/en-us/library/hh855347.aspx |
| NuiInitialize | https://learn.microsoft.com/en-us/previous-versions/windows/kinect-1.8/jj663949(v=ieb.10) | https://web.archive.org/web/20151001/https://msdn.microsoft.com/en-us/library/jj663949.aspx |
| INuiSensor interface | https://learn.microsoft.com/en-us/previous-versions/windows/kinect-1.8/hh855414(v=ieb.10) | https://web.archive.org/web/20150918/https://msdn.microsoft.com/en-us/library/hh855414.aspx |
| INuiSensor::NuiInitialize | https://learn.microsoft.com/en-us/previous-versions/windows/kinect-1.8/hh855438(v=ieb.10) | https://web.archive.org/web/20151001/https://msdn.microsoft.com/en-us/library/hh855438.aspx |
| INuiSensor::NuiSkeletonGetNextFrame | https://learn.microsoft.com/en-us/previous-versions/windows/kinect-1.8/hh855487(v=ieb.10) | https://web.archive.org/web/20151001/https://msdn.microsoft.com/en-us/library/hh855487.aspx |
| INuiSensor::NuiSkeletonTrackingEnable | https://learn.microsoft.com/en-us/previous-versions/windows/kinect-1.8/hh855491(v=ieb.10) | https://web.archive.org/web/20151001/https://msdn.microsoft.com/en-us/library/hh855491.aspx |
| INuiSensor::NuiCameraElevationGetAngle | https://learn.microsoft.com/en-us/previous-versions/windows/kinect-1.8/hh855418(v=ieb.10) | https://web.archive.org/web/20151001/https://msdn.microsoft.com/en-us/library/hh855418.aspx |
| INuiSensor::NuiCameraElevationSetAngle | https://learn.microsoft.com/en-us/previous-versions/windows/kinect-1.8/hh855417(v=ieb.10) | https://web.archive.org/web/20151001/https://msdn.microsoft.com/en-us/library/hh855417.aspx |
| NuiGetSensorCount | https://learn.microsoft.com/en-us/previous-versions/windows/kinect-1.8/hh855600(v=ieb.10) | https://web.archive.org/web/20151001/https://msdn.microsoft.com/en-us/library/hh855600.aspx |
| NuiCreateSensorByIndex | https://learn.microsoft.com/en-us/previous-versions/windows/kinect-1.8/hh855587(v=ieb.10) | https://web.archive.org/web/20151001/https://msdn.microsoft.com/en-us/library/hh855587.aspx |
| NuiCreateSensorById | https://learn.microsoft.com/en-us/previous-versions/windows/kinect-1.8/hh855586(v=ieb.10) | https://web.archive.org/web/20151001/https://msdn.microsoft.com/en-us/library/hh855586.aspx |
| NUI_SKELETON_DATA | https://learn.microsoft.com/en-us/previous-versions/windows/kinect-1.8/hh855537(v=ieb.10) | https://web.archive.org/web/20151001/https://msdn.microsoft.com/en-us/library/hh855537.aspx |
| NUI_SKELETON_FRAME | https://learn.microsoft.com/en-us/previous-versions/windows/kinect-1.8/hh855539(v=ieb.10) | https://web.archive.org/web/20151001/https://msdn.microsoft.com/en-us/library/hh855539.aspx |
| NUI_SKELETON_TRACKING_STATE | https://learn.microsoft.com/en-us/previous-versions/windows/kinect-1.8/hh855563(v=ieb.10) | https://web.archive.org/web/20151001/https://msdn.microsoft.com/en-us/library/hh855563.aspx |
| NUI_JOINT_TYPE | https://learn.microsoft.com/en-us/previous-versions/windows/kinect-1.8/hh855535(v=ieb.10) | https://web.archive.org/web/20151001/https://msdn.microsoft.com/en-us/library/hh855535.aspx |

### SDK Download Links (archived)

- Kinect for Windows SDK 1.8: https://www.microsoft.com/en-us/download/details.aspx?id=40278
- Kinect for Windows Developer Toolkit 1.8: https://www.microsoft.com/en-us/download/details.aspx?id=40276
- Archive reference: https://web.archive.org/web/2015*/https://msdn.microsoft.com/en-us/library/hh855347.aspx

### Header File Locations (after SDK install)

After installing Kinect SDK 1.8, headers are typically at:
- `C:\Program Files\Microsoft SDKs\Kinect\v1.8\inc\NuiApi.h`
- `C:\Program Files\Microsoft SDKs\Kinect\v1.8\inc\NuiSensor.h`
- `C:\Program Files\Microsoft SDKs\Kinect\v1.8\inc\NuiImageCamera.h`
- `C:\Program Files\Microsoft SDKs\Kinect\v1.8\inc\NuiSkeleton.h`
- `C:\Program Files\Microsoft SDKs\Kinect\v1.8\inc\NuiAudio.h`

Library:
- `C:\Program Files\Microsoft SDKs\Kinect\v1.8\lib\amd64\Kinect10.lib`

## Third-Party / Community Resources

| Resource | URL |
|----------|-----|
| OpenNI2 (open source Kinect driver) | https://github.com/structureio/openni2 |
| libfreenect (open source Kinect driver) | https://github.com/OpenKinect/libfreenect |
| libfreenect2 (Kinect v2, reference) | https://github.com/OpenKinect/libfreenect2 |
| Xbox 360 NUI reverse engineering notes | https://github.com/xenia-project/game-compatibility |

## NUI API Function Categories

### Device Enumeration & Initialization
- `NuiGetSensorCount(int* pCount)` → Get number of connected Kinect sensors
- `NuiCreateSensorByIndex(int index, INuiSensor** ppNuiSensor)` → Open sensor by index
- `NuiCreateSensorById(const OLECHAR* strInstanceId, INuiSensor** ppNuiSensor)` → Open by ID
- `INuiSensor::NuiInitialize(DWORD dwFlags)` → Initialize with capability flags
- `INuiSensor::NuiShutdown()` → Shutdown sensor
- `INuiSensor::NuiStatus()` → Get sensor status (HRESULT: S_OK = connected)
- `INuiSensor::NuiUniqueId()` → Get unique instance ID string
- `INuiSensor::NuiDeviceConnectionId()` → Get connection ID

### Skeleton Tracking
- `INuiSensor::NuiSkeletonTrackingEnable(HANDLE hNextFrameEvent, DWORD dwFlags)` → Start tracking
- `INuiSensor::NuiSkeletonTrackingDisable()` → Stop tracking
- `INuiSensor::NuiSkeletonGetNextFrame(DWORD dwMillisecondsToWait, NUI_SKELETON_FRAME* pSkeletonFrame)` → Get skeleton frame
- `INuiSensor::NuiSkeletonSetTrackedSkeletons(DWORD TrackingIDs[2])` → Force-track specific skeleton IDs
- `NuiSkeletonCalculateBoneOrientations(const NUI_SKELETON_DATA* pSkeletonData, NUI_SKELETON_BONE_ORIENTATION* pBoneOrientations)` → Calculate bone orientations
- `NuiTransformSmooth(NUI_SKELETON_FRAME* pSkeletonFrame, const NUI_TRANSFORM_SMOOTH_PARAMETERS* pSmoothingParams)` → Smooth skeleton data

### Camera Elevation (Tilt)
- `INuiSensor::NuiCameraElevationGetAngle(LONG* plAngleDegrees)` → Get current tilt angle (-27 to +27)
- `INuiSensor::NuiCameraElevationSetAngle(LONG lAngleDegrees)` → Set tilt angle

### Color / Depth / IR Image
- `INuiSensor::NuiImageStreamOpen(NUI_IMAGE_TYPE eImageType, NUI_IMAGE_RESOLUTION eResolution, DWORD dwImageFrameFlags, DWORD dwFrameLimit, HANDLE hNextFrameEvent, HANDLE* phStreamHandle)` → Open image stream
- `INuiSensor::NuiImageStreamGetNextFrame(HANDLE hStream, DWORD dwMillisecondsToWait, NUI_IMAGE_FRAME* pImageFrame)` → Get next image frame
- `INuiSensor::NuiImageStreamReleaseFrame(HANDLE hStream, NUI_IMAGE_FRAME* pImageFrame)` → Release frame
- `INuiSensor::NuiImageGetColorPixelCoordinatesFromDepthPixel(...)` → Map depth to color pixels

### Audio
- `INuiSensor::NuiGetAudioSource(INuiAudioBeam** ppDMO)` → Get audio source

### Accelerometer
- `INuiSensor::NuiAccelerometerGetCurrentReading(Vector4* pAcceleration)` → Read accelerometer

## NUI_INITIALIZE Flags (used with INuiSensor::NuiInitialize)

```c
#define NUI_INITIALIZE_FLAG_USES_DEPTH_AND_PLAYER_INDEX  0x00000001
#define NUI_INITIALIZE_FLAG_USES_COLOR                   0x00000002
#define NUI_INITIALIZE_FLAG_USES_SKELETON                0x00000008
#define NUI_INITIALIZE_FLAG_USES_DEPTH                   0x00000020
#define NUI_INITIALIZE_FLAG_USES_HIGH_QUALITY_COLOR      0x00000040
```

## NUI_SKELETON_TRACKING_STATE Values

```c
NUI_SKELETON_NOT_TRACKED  = 0
NUI_SKELETON_POSITION_ONLY = 1
NUI_SKELETON_TRACKED       = 2
```

## NUI_JOINT_TYPE Values (20 joints)

```c
NUI_SKELETON_POSITION_HIP_CENTER       = 0
NUI_SKELETON_POSITION_SPINE            = 1
NUI_SKELETON_POSITION_SHOULDER_CENTER  = 2
NUI_SKELETON_POSITION_HEAD             = 3
NUI_SKELETON_POSITION_SHOULDER_LEFT    = 4
NUI_SKELETON_POSITION_ELBOW_LEFT       = 5
NUI_SKELETON_POSITION_WRIST_LEFT       = 6
NUI_SKELETON_POSITION_HAND_LEFT        = 7
NUI_SKELETON_POSITION_SHOULDER_RIGHT   = 8
NUI_SKELETON_POSITION_ELBOW_RIGHT      = 9
NUI_SKELETON_POSITION_WRIST_RIGHT      = 10
NUI_SKELETON_POSITION_HAND_RIGHT       = 11
NUI_SKELETON_POSITION_HIP_LEFT         = 12
NUI_SKELETON_POSITION_KNEE_LEFT        = 13
NUI_SKELETON_POSITION_ANKLE_LEFT       = 14
NUI_SKELETON_POSITION_FOOT_LEFT        = 15
NUI_SKELETON_POSITION_HIP_RIGHT        = 16
NUI_SKELETON_POSITION_KNEE_RIGHT       = 17
NUI_SKELETON_POSITION_ANKLE_RIGHT      = 18
NUI_SKELETON_POSITION_FOOT_RIGHT       = 19
NUI_SKELETON_POSITION_COUNT            = 20
```

## NUI_SKELETON_FRAME Structure

```c
typedef struct _NUI_SKELETON_FRAME {
    LARGE_INTEGER       liTimeStamp;       // 8 bytes
    DWORD               dwFrameNumber;     // 4 bytes
    DWORD               dwFlags;           // 4 bytes
    Vector4             vFloorClipPlane;   // 16 bytes (xyzw floats)
    Vector4             vNormalToGravity;  // 16 bytes
    NUI_SKELETON_DATA   SkeletonData[NUI_SKELETON_COUNT]; // NUI_SKELETON_COUNT=6
} NUI_SKELETON_FRAME;
```

## NUI_SKELETON_DATA Structure

```c
typedef struct _NUI_SKELETON_DATA {
    NUI_SKELETON_TRACKING_STATE eTrackingState;
    DWORD                       dwTrackingID;
    DWORD                       dwEnrollmentIndex;  // PLAYER_INDEX_NONE if not enrolled
    DWORD                       dwUserIndex;
    Vector4                     Position;           // root position (hip center)
    Vector4                     SkeletonPositions[NUI_SKELETON_POSITION_COUNT]; // 20 joints
    NUI_SKELETON_POSITION_TRACKING_STATE eSkeletonPositionTrackingState[NUI_SKELETON_POSITION_COUNT];
    DWORD                       dwQualityFlags;
} NUI_SKELETON_DATA;
```

## Xbox 360 NUI Differences from Windows Kinect SDK 1.8

The Xbox 360 uses its own NUI API (`xam.xex`) which wraps the internal Kinect driver.
Key differences:
1. Xbox 360 uses **big-endian** format for all data structures.
2. The Xbox 360 supports up to **2 tracked skeletons** (same as Windows SDK).
3. Xbox 360 uses **6 skeleton slots** (same as Windows SDK – `NUI_SKELETON_COUNT = 6`).
4. Xbox 360 device status uses a different struct (`X_NUI_DEVICE_STATUS`, 24 bytes).
5. The Xbox 360 has Kinect identity/enrollment features (biometric sign-in) that have no direct Windows SDK equivalent.
6. Xbox 360 uses `XamNuiCameraElevation*` functions instead of `INuiSensor::NuiCameraElevation*`.
7. Xbox 360 has HUD integration (`XamNuiHud*`) for Kinect UI overlays.

## Key Mapping: Xbox 360 NUI → Windows Kinect SDK 1.8

| Xbox 360 XAM Function | Windows SDK 1.8 Equivalent |
|----------------------|---------------------------|
| `XamNuiGetDeviceStatus` | `INuiSensor::NuiStatus()` + custom struct |
| `XamNuiIsDeviceReady` | `INuiSensor::NuiStatus() == S_OK` |
| `XamNuiCameraElevationSetAngle` | `INuiSensor::NuiCameraElevationSetAngle()` |
| `XamNuiCameraElevationGetAngle` | `INuiSensor::NuiCameraElevationGetAngle()` |
| `XamNuiSkeletonGetBestSkeletonIndex` | analyze `NUI_SKELETON_FRAME` |
| `XamNuiGetDeviceSerialNumber` | `INuiSensor::NuiUniqueId()` |
| `XamNuiEnableChatMic` | `INuiSensor::NuiGetAudioSource()` |
| `XamNuiGetCameraIntrinsics` | No direct equiv (custom calibration data) |
| `XamNuiGetDepthCalibration` | No direct equiv (custom calibration data) |

## Planned Implementation Files

- `src/xenia/kernel/xam/xam_nui.cc` – Main implementation
- `src/xenia/kernel/xam/xam_nui.h` – (may be needed for shared structs if split)
- `src/xenia/nui/` – (potential new module for Windows Kinect SDK wrapper)
