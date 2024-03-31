/* functable.h -- Struct containing function pointers to optimized functions
 * Copyright (C) 2017 Hans Kristian Rosbach
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

#ifndef FUNCTABLE_H_
#define FUNCTABLE_H_

#include "deflate.h"
#include "crc32.h"

#  include "arch_functions.h"

#  define FUNCTABLE_INIT ((void)0)
#  define FUNCTABLE_CALL(name) native_ ## name
#  define FUNCTABLE_FPTR(name) &native_ ## name

#endif
