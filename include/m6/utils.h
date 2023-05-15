// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+matey6@pm.me>

#ifndef M6_UTILS_H
#define M6_UTILS_H

extern bool m6_verbose;

#if defined(__clang__) || defined(__GNUC__)
    #define M6_HAVE_GNU
#endif

#ifdef M6_HAVE_GNU
    #if __has_attribute(packed)
        #define M6_PACKED __attribute__((packed))
    #endif
    #if __has_attribute(noreturn)
        #define M6_NORETURN __attribute__((noreturn))
    #endif
#endif

void m6_fatal_errno(const char* msg);
void m6_verbose_printf(const char* fmt, ...);

#endif
