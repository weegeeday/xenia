# Kinect SDK 1.8 – Comprehensive API Research Notes
# (From real SDK headers sourced from public GitHub repositories)

## Source Files Researched

- `NuiSensor.h` (MIDL-generated COM interface definition) – `IID_INuiSensor`, `INuiSensor`, global functions
- `NuiSkeleton.h` – Skeleton structures, tracking enums, functions
- `NuiImageCamera.h` – Image types, resolutions, camera constants, image functions
- `MSR_NuiApi.h` – Legacy (v1.0/1.5) single-device API and `INuiInstance` (for reference only)
- `KinectFaker/Proxy.cpp` – Real-world usage pattern of `Kinect10.dll` runtime loading

---

## 1. Core Headers

The main include is `NuiApi.h`, which pulls in:
- `NuiSensor.h` – COM interfaces (`INuiSensor`, `INuiFrameTexture`, `INuiAudioBeam`)
- `NuiSkeleton.h` – Skeleton tracking structures/functions
- `NuiImageCamera.h` – Image/camera streaming

Library to link: `Kinect10.lib`
DLL at runtime: `Kinect10.dll` (typically in `%SystemRoot%\System32\`)

---

## 2. Initialization Flags (`NuiInitialize` flags)

```c
#define NUI_INITIALIZE_FLAG_USES_DEPTH_AND_PLAYER_INDEX  0x00000001
#define NUI_INITIALIZE_FLAG_USES_COLOR                   0x00000002
#define NUI_INITIALIZE_FLAG_USES_SKELETON                0x00000008
#define NUI_INITIALIZE_FLAG_USES_DEPTH                   0x00000020
#define NUI_INITIALIZE_FLAG_USES_HIGH_QUALITY_COLOR      0x00000040  // (1.8 only)
```

---

## 3. Error Codes

```c
// Win32-derived
#define E_NUI_DEVICE_NOT_CONNECTED   __HRESULT_FROM_WIN32(ERROR_DEVICE_NOT_CONNECTED)
#define E_NUI_DEVICE_NOT_READY       __HRESULT_FROM_WIN32(ERROR_NOT_READY)
#define E_NUI_ALREADY_INITIALIZED    __HRESULT_FROM_WIN32(ERROR_ALREADY_INITIALIZED)
#define E_NUI_NO_MORE_ITEMS          __HRESULT_FROM_WIN32(ERROR_NO_MORE_ITEMS)

// NUI-specific (FACILITY_NUI = 0x301)
#define E_NUI_FRAME_NO_DATA              MAKE_HRESULT(SEVERITY_ERROR, 0x301, 1)
#define E_NUI_STREAM_NOT_ENABLED         MAKE_HRESULT(SEVERITY_ERROR, 0x301, 2)
#define E_NUI_IMAGE_STREAM_IN_USE        MAKE_HRESULT(SEVERITY_ERROR, 0x301, 3)
#define E_NUI_FRAME_LIMIT_EXCEEDED       MAKE_HRESULT(SEVERITY_ERROR, 0x301, 4)
#define E_NUI_FEATURE_NOT_INITIALIZED    MAKE_HRESULT(SEVERITY_ERROR, 0x301, 5)
#define E_NUI_DATABASE_NOT_FOUND         MAKE_HRESULT(SEVERITY_ERROR, 0x301, 13)
#define E_NUI_DATABASE_VERSION_MISMATCH  MAKE_HRESULT(SEVERITY_ERROR, 0x301, 14)
#define E_NUI_BADINDEX                   MAKE_HRESULT(SEVERITY_ERROR, 0x301, ?)  // approx
```

---

## 4. Global Device Enumeration Functions (in `NuiSensor.h`)

```cpp
// Get number of connected Kinect sensors
HRESULT NUIAPI NuiGetSensorCount(int* pCount);

// Open sensor by 0-based index
HRESULT NUIAPI NuiCreateSensorByIndex(int index, INuiSensor** ppNuiSensor);

// Open sensor by instance ID (Unicode string)
HRESULT NUIAPI NuiCreateSensorById(const OLECHAR* strInstanceId, INuiSensor** ppNuiSensor);

// Get audio source (global, single-sensor compatibility)
HRESULT NUIAPI NuiGetAudioSource(INuiAudioBeam** ppDmo);

// Register a callback for device connect/disconnect events
typedef void (CALLBACK* NuiStatusProc)(HRESULT hrStatus, const OLECHAR* instanceName,
                                        const OLECHAR* uniqueDeviceName, void* pUserData);
void NUIAPI NuiSetDeviceStatusCallback(NuiStatusProc callback, void* pUserData);
```

---

## 5. `INuiSensor` COM Interface (`IID_INuiSensor = {1f5e088c-a8c7-41d3-9957-209677a13e85}`)

### Lifecycle
```cpp
HRESULT NuiInitialize(DWORD dwFlags);  // flags from NUI_INITIALIZE_FLAG_*
void    NuiShutdown();
HRESULT NuiStatus();                   // returns S_OK if connected & working
DWORD   NuiInitializationFlags();      // returns flags passed to NuiInitialize
int     NuiInstanceIndex();            // 0-based index this sensor was opened with
```

### Identity
```cpp
BSTR NuiUniqueId();          // unique device ID string (e.g., USB serial)
BSTR NuiDeviceConnectionId();// connection path (e.g., USB hub path)
BSTR NuiAudioArrayId();      // associated microphone array ID
```

### Events
```cpp
HRESULT NuiSetFrameEndEvent(HANDLE hEvent, DWORD dwFrameEventFlag);
```

### Camera Elevation (Tilt Motor)
```cpp
// Range: NUI_CAMERA_ELEVATION_MINIMUM (-27) to NUI_CAMERA_ELEVATION_MAXIMUM (+27) degrees
HRESULT NuiCameraElevationGetAngle(LONG* plAngleDegrees);
HRESULT NuiCameraElevationSetAngle(LONG lAngleDegrees);

#define NUI_CAMERA_ELEVATION_MAXIMUM   27
#define NUI_CAMERA_ELEVATION_MINIMUM  (-27)
```

### Skeleton Tracking
```cpp
HRESULT NuiSkeletonTrackingEnable(HANDLE hNextFrameEvent, DWORD dwFlags);
HRESULT NuiSkeletonTrackingDisable();
HRESULT NuiSkeletonGetNextFrame(DWORD dwMillisecondsToWait, NUI_SKELETON_FRAME* pSkeletonFrame);
HRESULT NuiSkeletonSetTrackedSkeletons(DWORD* TrackingIDs);  // array of 2 IDs
HRESULT NuiTransformSmooth(NUI_SKELETON_FRAME* pSkeletonFrame,
                            const NUI_TRANSFORM_SMOOTH_PARAMETERS* pSmoothingParams);
```

### Image Streaming
```cpp
HRESULT NuiImageStreamOpen(NUI_IMAGE_TYPE eImageType, NUI_IMAGE_RESOLUTION eResolution,
                            DWORD dwImageFrameFlags, DWORD dwFrameLimit,
                            HANDLE hNextFrameEvent, HANDLE* phStreamHandle);
HRESULT NuiImageStreamGetNextFrame(HANDLE hStream, DWORD dwMillisecondsToWait,
                                   NUI_IMAGE_FRAME* pImageFrame);
HRESULT NuiImageStreamReleaseFrame(HANDLE hStream, NUI_IMAGE_FRAME* pImageFrame);
HRESULT NuiImageStreamSetImageFrameFlags(HANDLE hStream, DWORD dwImageFrameFlags);
HRESULT NuiImageStreamGetImageFrameFlags(HANDLE hStream, DWORD* pdwImageFrameFlags);

// Depth-to-color coordinate mapping
HRESULT NuiImageGetColorPixelCoordinatesFromDepthPixel(
    NUI_IMAGE_RESOLUTION eColorResolution, const NUI_IMAGE_VIEW_AREA* pcViewArea,
    LONG lDepthX, LONG lDepthY, USHORT usDepthValue, LONG* plColorX, LONG* plColorY);
HRESULT NuiImageGetColorPixelCoordinatesFromDepthPixelAtResolution(
    NUI_IMAGE_RESOLUTION eColorResolution, NUI_IMAGE_RESOLUTION eDepthResolution,
    const NUI_IMAGE_VIEW_AREA* pcViewArea, LONG lDepthX, LONG lDepthY,
    USHORT usDepthValue, LONG* plColorX, LONG* plColorY);
HRESULT NuiImageGetColorPixelCoordinateFrameFromDepthPixelFrameAtResolution(
    NUI_IMAGE_RESOLUTION eColorResolution, NUI_IMAGE_RESOLUTION eDepthResolution,
    DWORD cDepthValues, USHORT* pDepthValues,
    DWORD cColorCoordinates, LONG* pColorCoordinates);
```

### Audio
```cpp
HRESULT NuiGetAudioSource(INuiAudioBeam** ppDmo);
```

---

## 6. Skeleton Structures & Enums

### Joint Positions (NUI_SKELETON_POSITION_INDEX, 20 joints + COUNT)
```c
NUI_SKELETON_POSITION_HIP_CENTER      = 0
NUI_SKELETON_POSITION_SPINE           = 1
NUI_SKELETON_POSITION_SHOULDER_CENTER = 2
NUI_SKELETON_POSITION_HEAD            = 3
NUI_SKELETON_POSITION_SHOULDER_LEFT   = 4
NUI_SKELETON_POSITION_ELBOW_LEFT      = 5
NUI_SKELETON_POSITION_WRIST_LEFT      = 6
NUI_SKELETON_POSITION_HAND_LEFT       = 7
NUI_SKELETON_POSITION_SHOULDER_RIGHT  = 8
NUI_SKELETON_POSITION_ELBOW_RIGHT     = 9
NUI_SKELETON_POSITION_WRIST_RIGHT     = 10
NUI_SKELETON_POSITION_HAND_RIGHT      = 11
NUI_SKELETON_POSITION_HIP_LEFT        = 12
NUI_SKELETON_POSITION_KNEE_LEFT       = 13
NUI_SKELETON_POSITION_ANKLE_LEFT      = 14
NUI_SKELETON_POSITION_FOOT_LEFT       = 15
NUI_SKELETON_POSITION_HIP_RIGHT       = 16
NUI_SKELETON_POSITION_KNEE_RIGHT      = 17
NUI_SKELETON_POSITION_ANKLE_RIGHT     = 18
NUI_SKELETON_POSITION_FOOT_RIGHT      = 19
NUI_SKELETON_POSITION_COUNT           = 20
```

### Per-Joint Tracking State
```c
NUI_SKELETON_POSITION_NOT_TRACKED = 0  // joint not visible
NUI_SKELETON_POSITION_INFERRED    = 1  // joint inferred from context
NUI_SKELETON_POSITION_TRACKED     = 2  // joint actively tracked
```

### Per-Skeleton Tracking State
```c
NUI_SKELETON_NOT_TRACKED   = 0
NUI_SKELETON_POSITION_ONLY = 1  // only hip center position known
NUI_SKELETON_TRACKED       = 2  // full skeleton tracked
```

### NUI_SKELETON_DATA (per skeleton slot)
```c
typedef struct _NUI_SKELETON_DATA {
    NUI_SKELETON_TRACKING_STATE  eTrackingState;             // 4 bytes
    DWORD                        dwTrackingID;               // 4 bytes (0 = invalid)
    DWORD                        dwEnrollmentIndex;          // 4 bytes
    DWORD                        dwUserIndex;                // 4 bytes
    Vector4                      Position;                   // 16 bytes (hip center)
    Vector4                      SkeletonPositions[20];      // 320 bytes (xyzw per joint)
    NUI_SKELETON_POSITION_TRACKING_STATE
                                 eSkeletonPositionTrackingState[20]; // 80 bytes
    DWORD                        dwQualityFlags;             // 4 bytes
} NUI_SKELETON_DATA;
// Total: ~432 bytes per skeleton
```

### NUI_SKELETON_FRAME (pack 16)
```c
#pragma pack(push, 16)
typedef struct _NUI_SKELETON_FRAME {
    LARGE_INTEGER   liTimeStamp;         // 8 bytes
    DWORD           dwFrameNumber;       // 4 bytes
    DWORD           dwFlags;             // 4 bytes
    Vector4         vFloorClipPlane;     // 16 bytes (ax+by+cz+d=0 plane eq)
    Vector4         vNormalToGravity;    // 16 bytes (up vector)
    NUI_SKELETON_DATA SkeletonData[6];   // 6 skeletons
} NUI_SKELETON_FRAME;
#pragma pack(pop)

#define NUI_SKELETON_COUNT            6
#define NUI_SKELETON_MAX_TRACKED_COUNT 2
#define NUI_SKELETON_INVALID_TRACKING_ID 0
```

### Skeleton Quality Flags
```c
#define NUI_SKELETON_QUALITY_CLIPPED_RIGHT  0x00000001
#define NUI_SKELETON_QUALITY_CLIPPED_LEFT   0x00000002
#define NUI_SKELETON_QUALITY_CLIPPED_TOP    0x00000004
#define NUI_SKELETON_QUALITY_CLIPPED_BOTTOM 0x00000008
```

### Skeleton Tracking Flags (for NuiSkeletonTrackingEnable)
```c
#define NUI_SKELETON_TRACKING_FLAG_SUPPRESS_NO_FRAME_DATA       0x00000001
#define NUI_SKELETON_TRACKING_FLAG_TITLE_SETS_TRACKED_SKELETONS 0x00000002
```

### NUI_TRANSFORM_SMOOTH_PARAMETERS
```c
typedef struct _NUI_TRANSFORM_SMOOTH_PARAMETERS {
    FLOAT fSmoothing;           // [0..1], lower = closer to raw data
    FLOAT fCorrection;          // [0..1], lower = slower to correct toward raw
    FLOAT fPrediction;          // [0..n], frames to predict into future
    FLOAT fJitterRadius;        // radius in meters for jitter reduction
    FLOAT fMaxDeviationRadius;  // max radius in meters filtered pos can deviate
} NUI_TRANSFORM_SMOOTH_PARAMETERS;
```

---

## 7. Image Structures & Enums

### NUI_IMAGE_TYPE
```c
NUI_IMAGE_TYPE_DEPTH_AND_PLAYER_INDEX = 0  // USHORT, depth + player bits
NUI_IMAGE_TYPE_COLOR                  = 1  // RGB32
NUI_IMAGE_TYPE_COLOR_YUV              = 2  // YUY2 converted to RGB32
NUI_IMAGE_TYPE_COLOR_RAW_YUV          = 3  // YUY2 raw from hardware
NUI_IMAGE_TYPE_DEPTH                  = 4  // USHORT, depth only
```

### NUI_IMAGE_RESOLUTION
```c
NUI_IMAGE_RESOLUTION_INVALID  = -1
NUI_IMAGE_RESOLUTION_80x60    = 0
NUI_IMAGE_RESOLUTION_320x240  = 1
NUI_IMAGE_RESOLUTION_640x480  = 2
NUI_IMAGE_RESOLUTION_1280x960 = 3  // high-res color only
```

### NUI_IMAGE_FRAME
```c
typedef struct _NUI_IMAGE_FRAME {
    LARGE_INTEGER        liTimeStamp;
    DWORD                dwFrameNumber;
    NUI_IMAGE_TYPE       eImageType;
    NUI_IMAGE_RESOLUTION eResolution;
    INuiFrameTexture*    pFrameTexture;
    DWORD                dwFrameFlags;
    NUI_IMAGE_VIEW_AREA  ViewArea;
} NUI_IMAGE_FRAME;
```

### Depth Pixel Encoding
```c
#define NUI_IMAGE_PLAYER_INDEX_SHIFT      3
#define NUI_IMAGE_PLAYER_INDEX_MASK       0x7   // ((1<<3)-1)
#define NUI_IMAGE_DEPTH_MAXIMUM           ((4000 << 3) | 0x7)
#define NUI_IMAGE_DEPTH_MINIMUM           (800 << 3)

// Helpers (from header)
inline USHORT NuiDepthPixelToDepth(USHORT p) { return p >> 3; }        // in mm
inline USHORT NuiDepthPixelToPlayerIndex(USHORT p) { return p & 0x7; } // 0=no player
```

### Camera Intrinsics / FOV
```c
// Depth sensor
NUI_CAMERA_DEPTH_NOMINAL_FOCAL_LENGTH_IN_PIXELS          285.63f  // 320x240 basis
NUI_CAMERA_DEPTH_NOMINAL_INVERSE_FOCAL_LENGTH_IN_PIXELS  3.501e-3f
NUI_CAMERA_DEPTH_NOMINAL_DIAGONAL_FOV                    70.0f  degrees
NUI_CAMERA_DEPTH_NOMINAL_HORIZONTAL_FOV                  58.5f  degrees
NUI_CAMERA_DEPTH_NOMINAL_VERTICAL_FOV                    45.6f  degrees

// Color sensor
NUI_CAMERA_COLOR_NOMINAL_FOCAL_LENGTH_IN_PIXELS          531.15f  // 640x480 basis
NUI_CAMERA_COLOR_NOMINAL_INVERSE_FOCAL_LENGTH_IN_PIXELS  1.83e-3f
NUI_CAMERA_COLOR_NOMINAL_DIAGONAL_FOV                    73.9f  degrees
NUI_CAMERA_COLOR_NOMINAL_HORIZONTAL_FOV                  62.0f  degrees
NUI_CAMERA_COLOR_NOMINAL_VERTICAL_FOV                    48.6f  degrees
```

### Image Stream Flags
```c
#define NUI_IMAGE_STREAM_FLAG_SUPPRESS_NO_FRAME_DATA         0x00010000
#define NUI_IMAGE_STREAM_FLAG_ENABLE_NEAR_MODE               0x00020000
#define NUI_IMAGE_STREAM_FLAG_DISTINCT_OVERFLOW_DEPTH_VALUES 0x00040000
#define NUI_IMAGE_STREAM_FRAME_LIMIT_MAXIMUM                 4  // max frames you can hold
```

---

## 8. Audio Interface (`INuiAudioBeam`, `IID_INuiAudioBeam = {8c3cebfa-a35d-497e-bc9a-e9752a8155e0}`)

```cpp
interface INuiAudioBeam : public IUnknown {
    HRESULT GetBeam(double* angle);           // current beam angle (radians, -1..+1 radians range)
    HRESULT SetBeam(double angle);            // set beam direction
    HRESULT GetPosition(double* angle, double* confidence); // detected sound position
};
```

---

## 9. Frame Texture Interface (`INuiFrameTexture`, `IID_INuiFrameTexture = {13ea17f5-ff2e-4670-9ee5-1297a6e880d1}`)

```cpp
interface INuiFrameTexture : public IUnknown {
    int     BufferLen();
    int     Pitch();
    HRESULT LockRect(UINT Level, NUI_LOCKED_RECT* pLockedRect, RECT* pRect, DWORD Flags);
    HRESULT GetLevelDesc(UINT Level, NUI_SURFACE_DESC* pDesc);
    HRESULT UnlockRect(UINT Level);
};

typedef struct _NUI_LOCKED_RECT {
    INT   Pitch;   // bytes per row
    int   size;    // total bytes in pBits
    BYTE* pBits;
} NUI_LOCKED_RECT;

typedef struct _NUI_SURFACE_DESC {
    UINT Width;
    UINT Height;
} NUI_SURFACE_DESC;
```

---

## 10. Coordinate Space Conversion Utilities

These are inline functions in `NuiSkeleton.h`:

```cpp
// 3D skeleton point → 2D depth image pixel
void NuiTransformSkeletonToDepthImage(Vector4 vPoint,
    LONG* plDepthX, LONG* plDepthY, USHORT* pusDepthValue,
    NUI_IMAGE_RESOLUTION eResolution = NUI_IMAGE_RESOLUTION_320x240);

// 2D depth pixel → 3D skeleton point
Vector4 NuiTransformDepthImageToSkeleton(LONG lDepthX, LONG lDepthY,
    USHORT usDepthValue,
    NUI_IMAGE_RESOLUTION eResolution = NUI_IMAGE_RESOLUTION_320x240);
```

Conversion math (based on 320x240):
```
x_meters = (x_pixel - 160) * NUI_CAMERA_DEPTH_IMAGE_TO_SKELETON_MULTIPLIER_320x240 * z_meters
y_meters = (y_pixel - 120) * NUI_CAMERA_DEPTH_IMAGE_TO_SKELETON_MULTIPLIER_320x240 * z_meters
```

---

## 11. Additional Global Functions (Kinect SDK 1.8)

```cpp
// Bone orientations (requires linking against Kinect10.lib)
HRESULT NuiSkeletonCalculateBoneOrientations(
    const NUI_SKELETON_DATA* pSkeletonData,
    NUI_SKELETON_BONE_ORIENTATION* pBoneOrientations  // array of NUI_SKELETON_POSITION_COUNT
);

// Coordinate mapper (1.8 new feature)
HRESULT NuiCreateCoordinateMapperFromParameters(
    ULONG dataByteCount, void* pData, INuiCoordinateMapper** ppCoordinateMapper);

// Depth filter (1.8 new feature)
HRESULT NuiCreateDepthFilter(
    LPCWSTR filename, LPCSTR factoryEntryPoint, INuiDepthFilter** ppDepthFilter);

// Microphone array enumeration
HRESULT NuiGetMicrophoneArrayDevices(
    PNUI_MICROPHONE_ARRAY_DEVICE pDeviceInfo, int size, int* piDeviceCount);

// Speaker enumeration
HRESULT NuiGetSpeakerDevices(
    PNUI_SPEAKER_DEVICE pDeviceInfo, int size, int* piDeviceCount);
```

---

## 12. Runtime DLL Loading Pattern

From `KinectFaker/Proxy.cpp`, the canonical way to use `Kinect10.dll` without hard-linking:

```cpp
// Load DLL from System32
TCHAR systemdir[MAX_PATH];
GetSystemDirectory(systemdir, MAX_PATH);
HMODULE kinectHndl = LoadLibraryEx(
    std::wstring(systemdir) + L"\\Kinect10.dll", NULL, 0);

// Get function pointer
typedef HRESULT (WINAPI* PFN_NuiGetSensorCount)(int*);
auto pfn = (PFN_NuiGetSensorCount)GetProcAddress(kinectHndl, "NuiGetSensorCount");

// Use
int count = 0;
pfn(&count);  // 0 if no Kinect, 1+ if connected
```

**S_OK** (0) from `INuiSensor::NuiStatus()` = device connected and working.
Any non-S_OK = error/disconnected.

---

## 13. Typical Usage Pattern

```cpp
// 1. Check for sensors
int sensorCount = 0;
NuiGetSensorCount(&sensorCount);
if (sensorCount == 0) { /* not connected */ }

// 2. Create sensor
INuiSensor* pSensor = nullptr;
NuiCreateSensorByIndex(0, &pSensor);

// 3. Check status
if (FAILED(pSensor->NuiStatus())) { /* error */ }

// 4. Initialize with skeleton tracking
pSensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_SKELETON);

// 5. Enable skeleton tracking
HANDLE hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
pSensor->NuiSkeletonTrackingEnable(hEvent, 0);

// 6. Get frames in a loop
while (running) {
    NUI_SKELETON_FRAME frame = {};
    if (SUCCEEDED(pSensor->NuiSkeletonGetNextFrame(0, &frame))) {
        for (int i = 0; i < NUI_SKELETON_COUNT; i++) {
            if (frame.SkeletonData[i].eTrackingState == NUI_SKELETON_TRACKED) {
                // Use frame.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HEAD]
            }
        }
    }
}

// 7. Cleanup
pSensor->NuiSkeletonTrackingDisable();
pSensor->NuiShutdown();
pSensor->Release();
```

---

## 14. NUI_SKELETON_BONE_ORIENTATION (from NuiSkeletonBoneOrientation.h)

Note: This is a 1.8 feature not present in early SDK versions.

```c
typedef struct _NUI_SKELETON_BONE_ORIENTATION {
    NUI_SKELETON_POSITION_INDEX startJoint;
    NUI_SKELETON_POSITION_INDEX endJoint;
    // Rotation relative to parent bone
    NUI_SKELETON_BONE_ROTATION hierarchicalRotation;
    // Rotation relative to camera/absolute coordinates
    NUI_SKELETON_BONE_ROTATION absoluteRotation;
} NUI_SKELETON_BONE_ORIENTATION;

typedef struct _NUI_SKELETON_BONE_ROTATION {
    Matrix4 rotationMatrix;  // 4x4 rotation matrix
    Vector4 rotationQuaternion;  // xyzw quaternion
} NUI_SKELETON_BONE_ROTATION;
```

---

## 15. Accelerometer (INuiSensor – extended in some versions)

Some Kinect SDK versions expose accelerometer data. The Xbox 360 Kinect always has one.
In SDK 1.8, there may be a `NuiAccelerometerGetCurrentReading(Vector4*)` method but it is
not in the core `INuiSensor` interface – it may be in an extended interface or a separate header.

---

## 16. Device Status HRESULT Values

`INuiSensor::NuiStatus()` returns an HRESULT:
- `S_OK` (0) = device connected and initialized correctly
- `E_NUI_DEVICE_NOT_CONNECTED` = no device
- `E_NUI_DEVICE_NOT_READY` = device present but not ready (initializing)
- `E_NUI_ALREADY_INITIALIZED` = already initialized

---

## 17. Xbox 360 ↔ Windows SDK Data Alignment Notes

The Xbox 360 is **big-endian (PowerPC)**. All data from the Xbox game will be big-endian.
The Windows Kinect SDK works in **little-endian (x86/x64)**.

When translating `NUI_SKELETON_FRAME` from Windows SDK to Xbox 360:
- Swap all `DWORD`, `LONG`, `float` values byte-by-byte
- `LARGE_INTEGER` (64-bit) swap
- `Vector4` (4× float) – swap each float individually
- Enum values → treat as `DWORD` and swap

The Xbox 360 has `NUI_SKELETON_COUNT = 6` and `NUI_SKELETON_POSITION_COUNT = 20` – same as Windows SDK. ✓

---

## 18. Key Mapping Summary for Xenia Implementation

| Xbox 360 XAM Function | Windows SDK 1.8 Call | Notes |
|----------------------|---------------------|-------|
| `XamNuiGetDeviceStatus` | `NuiGetSensorCount()` + `NuiStatus()` | Fill `X_NUI_DEVICE_STATUS.status` |
| `XamNuiIsDeviceReady` | `NuiStatus() == S_OK` | Returns bool (1=ready) |
| `XamNuiCameraElevationGetAngle` | `NuiCameraElevationGetAngle()` | Returns LONG (-27..+27) |
| `XamNuiCameraElevationSetAngle` | `NuiCameraElevationSetAngle()` | LONG clamp to ±27 |
| `XamNuiGetDeviceSerialNumber` | `NuiUniqueId()` | BSTR → ASCII/UTF-8 |
| `XamNuiSkeletonGetBestSkeletonIndex` | Analyze `NUI_SKELETON_FRAME` | Find best-quality tracked skeleton |
| `XamNuiHudGetEngagedTrackingID` | Track `dwTrackingID` | Global state |
| `XamNuiHudSetEngagedTrackingID` | `NuiSkeletonSetTrackedSkeletons()` | Set preferred tracking ID |
| `XamNuiEnableChatMic` | `NuiGetAudioSource()` | Enable audio DMO |
| `XamNuiGetCameraIntrinsics` | Hardcoded from `NuiImageCamera.h` | Focal lengths/FOV constants |
| `XamNuiGetFanRate` | No equivalent | Return 0 / stub |
| `XamNuiNatalCameraUpdateStarting/Complete` | No equivalent | Firmware hooks, stub OK |
| `XamKinectGetHardwareType` | No direct equivalent | Detect from `NuiUniqueId()` |

---

## 19. Implementation Notes for Xenia

### Global State Needed
```cpp
// In xam_nui.cc:
static INuiSensor* g_nui_sensor = nullptr;  // active sensor (Windows only)
static bool g_nui_skeleton_enabled = false;
static int g_nui_engaged_tracking_id = 0;
static LONG g_nui_camera_angle = 0;         // cached, updated from SDK
```

### Windows Platform Guard
```cpp
#if XE_PLATFORM_WIN32
#include <NuiApi.h>
// OR: dynamic loading via GetProcAddress on Kinect10.dll
#endif
```

### Dynamic Loading (recommended – no hard SDK dependency)
```cpp
// At startup:
typedef HRESULT (WINAPI* PFN_NuiGetSensorCount)(int*);
typedef HRESULT (WINAPI* PFN_NuiCreateSensorByIndex)(int, INuiSensor**);
// ... etc

static PFN_NuiGetSensorCount pfnNuiGetSensorCount = nullptr;
static HMODULE g_kinect_dll = nullptr;

void NuiLoad() {
    g_kinect_dll = LoadLibraryW(L"Kinect10.dll");
    if (g_kinect_dll) {
        pfnNuiGetSensorCount = (PFN_NuiGetSensorCount)
            GetProcAddress(g_kinect_dll, "NuiGetSensorCount");
        // ...etc
    }
}
```
