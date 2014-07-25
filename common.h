// This file is part of the PiEMU Project
// Licensing information can be found in the LICENSE file
// (C) 2014 Nandor Licker. All rights reserved.
#ifndef __COMMON_H__
#define __COMMON_H__


// C includes
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>


// C++ includes
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <exception>
#include <stdexcept>
#include <iomanip>
#include <streambuf>


// Linux includes
#include <ftw.h>


// Compiler specific macros
#if defined(__GNUC__)
# define FORCEINLINE    __attribute__((always_inline))
# define BSWAP_16(x)    __builtin_bswap16(x)
# define BSWAP_32(x)    __builtin_bswap32(x)
# define TRAP()         __builtin_trap()
# define UNREACHABLE()  __builtin_unreachable()
#else
# error "Unsupported compiler"
#endif


// Emulator includes
#include "exception.h"
#include "memory.h"
#include "arch/arm.h"
#include "arch/thumb.h"
#include "emulator.h"


#endif /*__COMMON_H__*/
