// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+matey6@pm.me>

#ifndef M6_UTILS_H
#define M6_UTILS_H

extern bool m6_verbose;

void m6_fatal_errno(const char* msg);
void m6_verbose_printf(const char* fmt, ...);

#endif
