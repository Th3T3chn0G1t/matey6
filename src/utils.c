// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+matey6@pm.me>

bool m6_verbose = false;

void m6_fatal_errno(const char* msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

#define M6_FORWARD_VPRINTF(s) \
    va_list ap; \
    va_start(ap, fmt); \
    int result = vfprintf(s, fmt, ap); \
    if(result < 0) m6_fatal_errno("vprintf"); \
    va_end(ap);

void m6_verbose_printf(const char* fmt, ...) {
    if(!m6_verbose) return;
    M6_FORWARD_VPRINTF(stdout)
}

void m6_fatal_printf(const char* fmt, ...) {
    M6_FORWARD_VPRINTF(stderr)
    exit(EXIT_FAILURE);
}
