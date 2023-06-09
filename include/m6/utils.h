/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+matey6@pm.me>
 */

#ifndef M6_UTILS_H
#define M6_UTILS_H

#if defined(__clang__) || defined(__GNUC__)
    #define M6_HAVE_GNU
#endif

#ifdef __WATCOMC__
	#define M6_HAVE_WATCOM
#endif

#ifdef M6_HAVE_GNU
    #if __has_attribute(noreturn)
        #define M6_NORETURN __attribute__((noreturn))
    #endif
    #if __has_attribute(format)
        #define M6_PRINTF(fmt, va) __attribute__((format(printf, fmt, va)))
    #endif
    #if __has_attribute(fallthrough)
        #define M6_FALLTHROUGH __attribute__((fallthrough))
    #endif
#endif

#ifdef M6_HAVE_WATCOM
	#define M6_NORETURN __declspec(aborts)
#endif

#ifndef M6_NORETURN
    #define M6_NORETURN
#endif

#ifndef M6_PRINTF
    #define M6_PRINTF(...)
#endif

#ifndef M6_FALLTHROUGH
    #define M6_FALLTHROUGH
#endif

typedef unsigned char m6_bool_t;
#define M6_TRUE 1
#define M6_FALSE 0

typedef unsigned int m6_ubitfield_t;
typedef signed int m6_bitfield_t;

extern m6_bool_t m6_verbose;

void m6_fatal_errno(const char* msg);
M6_PRINTF(1, 2) void m6_verbose_printf(const char* fmt, ...);
M6_PRINTF(1, 2) M6_NORETURN void m6_fatal_printf(const char* fmt, ...);

#endif
