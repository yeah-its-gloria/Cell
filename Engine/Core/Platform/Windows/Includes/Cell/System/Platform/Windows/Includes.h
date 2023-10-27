// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

// Disables seldom used methods
#define WIN32_LEAN_AND_MEAN 1

// Enables Unicode support
#define UNICODE 1

// Enables OCR_ constants
#define OEMRESOURCE 1

// Enables strict type checks
#define STRICT 1

// Enables some GUID functionality
#define INITGUID 1

#ifndef __cplusplus
// Enables C interfaces for COM classes
#define CINTERFACE 1

// Enables function call macros for COM classes
#define COBJMACROS 1

// Fixes problems with COM compatibility and C
#define MICROSOFT_WINDOWS_WINBASE_H_DEFINE_INTERLOCKED_CPLUSPLUS_OVERLOADS 1
#endif

// Disables various features across Windows' headers

#define NO                1
#define NOATOM            1
#define NOCLIPBOARD       1
#define NOCOLOR           1
#define NOCOMM            1
#define NOCRYPT           1
#define NODRAWTEXT        1
#define NODEFERWINDOWPOS  1
#define NOHELP            1
#define NOGDICAPMASKS     1
#define NOICONS           1
#define NOIMAGE           1
#define NOIME             1
#define NOKANJI           1
#define NOKERNEL          1
#define NOKEYSTATES       1
#define NOMCX             1
#define NOMEMMGR          1
#define NOMENUS           1
#define NOMETAFILE        1
#define NOMINMAX          1
#define NOOPENFILE        1
#define NOPROFILER        1
#define NOPROXYSTUB       1
#define NORASTEROPS       1
#define NORPC             1
#define NOSCROLL          1
#define NOSERVICE         1
#define NOSHOWWINDOW      1
#define NOSOUND           1
#define NOSYSMETRICS      1
#define NOWH              1
#define NOWINRES          1
#define NOTAPE            1
#define NOTEXTMETRIC      1

#undef NOCTLMGR
#undef NOGDI
#undef NOMB
#undef NOMSG
#undef NONLS
#undef NOWINMESSAGES
#undef NOWINSTYLES
#undef NOSYSCOMMANDS
#undef NOUSER
#undef NOVIRTUALKEYCODES
#undef NOWINOFFSETS

#include <Windows.h>

// Fixes the HRESULT_FROM_WIN32 function to be a compiler intrinsic and thus makes it usable in switch statements and such

#ifndef __cplusplus
#undef HRESULT_FROM_WIN32
#define HRESULT_FROM_WIN32(x) __HRESULT_FROM_WIN32(x)
#endif

#undef CopyMemory
#undef Yield
