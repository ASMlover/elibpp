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

#ifndef __EL_CONFIG_HEADER_H__
#define __EL_CONFIG_HEADER_H__

#if !defined(PLATFORM_WIN) && !defined(PLATFORM_LINUX)
# if defined(_WINDOWS_) || defined(_MSC_VER) || defined(__MINGW32__)
#   define PLATFORM_WIN
# elif defined(WIN32_LEAN_AND_MEAN)
#   define PLATFORM_WIN
# elif defined(__linux__) || defined(__GNUC__)
#   define PLATFORM_LINUX
# else
#  error "nonsupport this platform !!!"
# endif
#endif

#if defined(_MSC_VER)
# define COMPILER_MSVC
#elif defined(__GNUC__)
# define COMPILER_GCC
#endif

#if defined(COMPILER_MSVC)
# if (_MSC_VER < 1700)
#   error "please use the high version of the compiler"
# endif
#elif defined(COMPILER_GCC)
# define GCC_VERSION  (__GNUC__ * 100 + __GNUC_MINOR__ * 10)
# if (GCC_VERSION < 470)
#   error "please use the high version of the compiler"
# endif
#endif

#if defined(PLATFORM_WIN)
# define USING_SELECT
#elif defined(PLATFORM_LINUX)
# define USING_EPOLL
#endif

#endif  // __EL_CONFIG_HEADER_H__
