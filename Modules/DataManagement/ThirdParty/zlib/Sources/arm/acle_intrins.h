#ifndef ARM_ACLE_INTRINS_H
#define ARM_ACLE_INTRINS_H

#include <stdint.h>
#ifdef _MSC_VER
#  include <intrin.h>
#elif defined(HAVE_ARM_ACLE_H)
#  include <arm_acle.h>
#endif

#ifdef ARM_ACLE
#if defined(__aarch64__)
#  define Z_TARGET_CRC Z_TARGET("+crc")
#else
#  define Z_TARGET_CRC
#endif
#endif

#endif // include guard ARM_ACLE_INTRINS_H
