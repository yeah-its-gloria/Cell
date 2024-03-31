/* arch_functions.h -- Arch-specific function prototypes.
 * Copyright (C) 2017 Hans Kristian Rosbach
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

#ifndef CPU_FUNCTIONS_H_
#define CPU_FUNCTIONS_H_

#include "zbuild.h"
#include "zutil.h"
#include "crc32.h"
#include "deflate.h"
#include "fallback_builtins.h"

#include "generic/generic_functions.h"

#if defined(X86_FEATURES)
#  include "x86/x86_functions.h"
#elif defined(ARM_FEATURES)
#  include "arm/arm_functions.h"
#endif

#endif
