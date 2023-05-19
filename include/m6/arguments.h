/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+matey6@pm.me>
 */

#ifndef M6_ARGUMENTS_H
#define M6_ARGUMENTS_H

#include "machine.h"

struct m6_opts {
    enum m6_pmem_mode memory_mode;
    char* binary;
    m6_bool_t zero_pmem;

    char* dump;

	m6_bool_t overwrite_reset_vector;
	m6_word_t reset_vector;
};

void m6_parse_arguments(int argc, char** argv, struct m6_opts* opts);

#endif
