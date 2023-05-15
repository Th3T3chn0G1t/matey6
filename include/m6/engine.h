// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+matey6@pm.me>

#ifndef M6_ENGINE_H
#define M6_ENGINE_H

#include "machine.h"

struct m6_engine_parameters {
    enum m6_pmem_mode pmem_mode;
    bool zero_pmem;

    uint8_t* binary;
    size_t binary_size;
};

struct m6_engine {
    uint8_t* pmem;

    union m6_regular_registers regular_registers;

	m6_word_t ip;
};

void m6_engine_create(struct m6_engine_parameters* parameters, struct m6_engine* engine);
void m6_engine_destroy(struct m6_engine* engine);

#endif
