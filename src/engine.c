// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+matey6@pm.me>

#include "m6/engine.h"

void m6_engine_create(struct m6_engine_parameters* parameters, struct m6_engine* engine) {
    engine->pmem = malloc(M6_PMEM_SIZE);
    if(!engine->pmem) m6_fatal_errno("malloc");
    if(parameters->zero_pmem) memset(engine->pmem, 0, M6_PMEM_SIZE);

    static const size_t size_constraints[] = {
            [M6_HIGHER_HALF_BINARY] = M6_UNSEGMENTED_MAX / 2
    };

    static const size_t placements[] = {
            [M6_HIGHER_HALF_BINARY] = (M6_UNSEGMENTED_MAX / 2) + 1
    };

    size_t size = parameters->binary_size;
    size_t constraint = size_constraints[parameters->pmem_mode];
    if(size > constraint) {
        m6_fatal_printf(
                "binary of size %zu exceeds memory mode maximum of %zu\n",
                size, constraint
        );
    }

    uint8_t* pmem = engine->pmem;
    uint8_t* binary = parameters->binary;
    uint8_t* placement = pmem + placements[parameters->pmem_mode];
    size_t binary_size = parameters->binary_size;
    switch(parameters->pmem_mode) {
        case M6_HIGHER_HALF_BINARY: {
            memcpy(placement, binary, binary_size);
            memset(placement + binary_size, 0, constraint - binary_size);
            break;
        }
    }

	if(parameters->overwrite_reset_vector) {
		pmem[0xFFFE] = parameters->reset_vector >> 8ULL;
		pmem[0xFFFF] = parameters->reset_vector & 0xFF;
	}

	engine->ip = pmem[0xFFFE] << 8ULL | pmem[0xFFFF];
}

void m6_engine_destroy(struct m6_engine* engine) {
    free(engine->pmem);
}
