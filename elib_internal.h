// Copyright (c) 2013 ASMlover. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//  * Redistributions of source code must retain the above copyright
//    notice, this list ofconditions and the following disclaimer.
//
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in
//    the documentation and/or other materialsprovided with the
//    distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
// ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
#ifndef __ELIB_INTERNAL_HEADER_H__
#define __ELIB_INTERNAL_HEADER_H__

#include "el_config.h"
#if defined(PLATFORM_WIN)
# include <windows.h>

# include <direct.h>
# include <io.h>
# include <mmsystem.h>
# include <process.h>

# undef __func__
# define __func__             __FUNCSIG__

# define bzero(s, n)          memset((s), 0, (n))
# define bcopy(src, dest, n)  memcpy((dest), (src), (n))
# define bcmp(s1, s2, n)      memcmp((s1), (s2), (n))
#else
# include <sys/time.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <limits.h>
# include <pthread.h>
# include <unistd.h>

# define MAX_PATH PATH_MAX
# if defined(PLATFORM_MAC)
#   include <mach/mach.h>
#   include <mach/mach_time.h>
#   include <libkern/OSAtomic.h>
# endif
#endif
#include <sys/timeb.h>

#include <assert.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <queue>
#include <vector>

#if !defined(PLATFORM_WIN)
# include "./posix/el_posix_tools.h"
#endif

#include "el_static_assert.h"
#include "el_uncopyable.h"
#include "el_locker.h"
#include "el_singleton.h"
#include "el_unique_ptr.h"
#include "el_unique_array.h"

#include "el_allocator.h"
#include "el_condition.h"
#include "el_io.h"
#include "el_thread.h"

#endif  // __ELIB_INTERNAL_HEADER_H__
