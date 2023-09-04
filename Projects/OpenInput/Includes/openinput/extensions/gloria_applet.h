#pragma once

#include <openinput/openinput.h>

#ifdef __cplusplus
extern "C" {
#endif

#define OI_GLORIA_applet 1
#define OI_GLORIA_APPLET_SPEC_VERSION 1
#define OI_GLORIA_APPLET_EXTENSION_NAME "OI_GLORIA_applet"

typedef struct OiAppletCreateInfoGLORIA {
    OiStructureType type;
    const void* next;
} OiAppletCreateInfoGLORIA;

#ifdef _WIN32
#include <Windows.h>

typedef struct OiAppletWindowInfoWin32GLORIA {
    OiStructureType type;
    const void* next;
    HWND window;
} OiAppletWindowInfoWin32GLORIA;
#endif

typedef OiResult (* PFN_oiCreateAppletGLORIA)(OiInstance instance, const OiAppletCreateInfoGLORIA createInfo);
typedef OiResult (* PFN_oiRemoveAppletGLORIA)(OiInstance instance);

OI_FUNCTION OiResult oiCreateAppletGLORIA(OiInstance instance, const OiAppletCreateInfoGLORIA createInfo);
OI_FUNCTION OiResult oiRemoveAppletGLORIA(OiInstance instance);

#ifdef __cplusplus
}
#endif
