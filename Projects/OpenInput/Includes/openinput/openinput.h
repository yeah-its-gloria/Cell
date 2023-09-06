#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define OI_MAKE_VERSION(major, minor, patch) ((((major) & 0xffffull) << 48) | (((minor) & 0xffffull) << 32) | ((patch) & 0xffffffffull))

#define OI_CURRENT_API_VERSION OI_MAKE_VERSION(1, 0, 0)

#define OI_VERSION_MAJOR(version) (uint16_t)(((uint64_t)(version) >> 48)& 0xffffull)
#define OI_VERSION_MINOR(version) (uint16_t)(((uint64_t)(version) >> 32) & 0xffffull)
#define OI_VERSION_PATCH(version) (uint32_t)((uint64_t)(version) & 0xffffffffull)

#define OI_DEFINE_HANDLE(object) typedef struct object##_T* object;

#ifdef _WIN32
#define OI_FUNCTION __declspec(dllexport)
#else
#define OI_FUNCTION __attribute__((visibility("default")))
#endif

OI_DEFINE_HANDLE(OiInstance);
OI_DEFINE_HANDLE(OiDevice);

#define OI_MAX_ENGINE_NAME_SIZE      128
#define OI_MAX_APPLICATION_NAME_SIZE 128
#define OI_MAX_DEVICE_NAME           128

typedef enum OiResult {
    OI_SUCCESS = 0,
    OI_ERROR_INVALID_PARAMETERS = -1,

    OI_RESULT_MAX_ENUM = 0x7fffffff
} OiResult;

typedef enum OiStructureType {
    OI_STRUCTURE_TYPE_UNKNOWN = 0,
    OI_STRUCTURE_TYPE_INSTANCE_CREATE_INFO = 1,

    OI_STRUCTURE_TYPE_GLORIA_APPLET_CREATE_INFO = 10000,
    OI_STRUCTURE_TYPE_GLORIA_APPLET_WINDOW_INFO_WIN32 = 10001,

    OI_STRUCTURE_TYPE_MAX_ENUM = 0x7fffffff
} OiStructureType;

typedef enum OiActionType {
    OI_ACTION_TYPE_UNKNOWN = 0,

    OI_ACTION_TYPE_BOOLEAN_INPUT = 1,
    OI_ACTION_TYPE_FLOAT_INPUT = 2,
    OI_ACTION_TYPE_VECTOR2_FLOAT_INPUT = 3,
    OI_ACTION_TYPE_VECTOR3_FLOAT_INPUT = 4,
    OI_ACTION_TYPE_FLOAT_OUTPUT = 5,

    OI_ACTION_TYPE_MAX_ENUM = 0x7fffffff
} OiActionType;

typedef enum OiDeviceType {
    OI_DEVICE_TYPE_CONTROLLER,
    OI_DEVICE_TYPE_KEYBOARD,
    OI_DEVICE_TYPE_MOUSE
} OiDeviceType;

typedef struct OiApplicationInfo {
    char applicationName[OI_MAX_APPLICATION_NAME_SIZE];
    uint32_t applicationVersion;
    char engineName[OI_MAX_ENGINE_NAME_SIZE];
    uint32_t engineVersion;
    uint64_t apiVersion;
} OiApplicationInfo;

typedef struct OiInstanceCreateInfo {
    OiStructureType type;
    const void* next;
    const OiApplicationInfo applicationInfo;
    uint32_t enabledExtensionCount;
    const char* const* enabledExtensionNames;
} OiInstanceCreateInfo;

typedef struct OiActionSetCreateInfo {
    OiStructureType type;
    const void* next;
} OiActionSetCreateInfo;

typedef struct OiDeviceInfo {
    uint32_t apiVersion;
    uint32_t driverVersion;
    uint32_t vendorID;
    uint32_t deviceID;
    OiDeviceType deviceType;
    char deviceName[OI_MAX_DEVICE_NAME];
} OiDeviceInfo;

typedef void (* PFN_oiVoidFunction)(void);

typedef OiResult (* PFN_oiCreateInstance)(const OiInstanceCreateInfo createInfo, OiInstance* instance);
typedef OiResult (* PFN_oiDestroyInstance)(OiInstance instance);
typedef OiResult (* PFN_oiRequestUsers)(OiInstance instance, const uint8_t count);
typedef OiResult (* PFN_oiAcquireUpdates)(OiInstance instance);
typedef OiResult (* PFN_oiEnumerateDevices)(OiInstance instance, uint32_t* count, OiDevice* devices);
typedef OiResult (* PFN_oiGetDeviceInfo)(OiDevice device, OiDeviceInfo* info);

OI_FUNCTION PFN_oiVoidFunction oiGetInstanceProcAddr(OiInstance instance, const char* name);

OI_FUNCTION OiResult oiCreateInstance(const OiInstanceCreateInfo createInfo, OiInstance* instance);
OI_FUNCTION OiResult oiDestroyInstance(OiInstance instance);
OI_FUNCTION OiResult oiRequestUsers(OiInstance instance, const uint8_t count);
OI_FUNCTION OiResult oiAcquireUpdates(OiInstance instance);

OI_FUNCTION OiResult oiEnumerateDevices(OiInstance instance, uint32_t* count, OiDevice* devices);
OI_FUNCTION OiResult oiGetDeviceInfo(OiDevice device, OiDeviceInfo* info);

#ifdef __cplusplus
}
#endif
