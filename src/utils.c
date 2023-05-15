// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+matey6@pm.me>

bool m6_verbose = false;

void m6_fatal_errno(const char* msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

void m6_verbose_printf(const char* fmt, ...) {
    if(!m6_verbose) return;

    va_list ap;
    va_start(ap, fmt);
    int result = vprintf(fmt, ap);
    if(result < 0) m6_fatal_errno("vprinf");
    va_end(ap);
}
