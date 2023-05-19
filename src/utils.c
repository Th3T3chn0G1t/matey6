/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+matey6@pm.me>
 */

m6_bool_t m6_verbose = M6_FALSE;

void m6_fatal_errno(const char* msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

#define M6_FORWARD_VPRINTF(s) \
    va_start(ap, fmt); \
    result = vfprintf(s, fmt, ap); \
    if(result < 0) m6_fatal_errno("vprintf"); \
    va_end(ap);

void m6_verbose_printf(const char* fmt, ...) {
    va_list ap;
    int result;
    if(!m6_verbose) return;
    M6_FORWARD_VPRINTF(stdout)
}

void m6_fatal_printf(const char* fmt, ...) {
    va_list ap;
    int result;
    M6_FORWARD_VPRINTF(stderr)
    exit(EXIT_FAILURE);
}
