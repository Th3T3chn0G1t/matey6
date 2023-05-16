// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+matey6@pm.me>

#include "m6/engine.h"
#include "m6/basicops.h"

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
		pmem[M6_RESET_VECTOR] = parameters->reset_vector >> 8ULL;
		pmem[M6_RESET_VECTOR + 1] = parameters->reset_vector & 0xFF;
	}

	engine->ip = pmem[M6_RESET_VECTOR] << 8ULL | pmem[M6_RESET_VECTOR + 1];
}

void m6_engine_destroy(struct m6_engine* engine) {
    free(engine->pmem);
}

static uint8_t m6_engine_segmented_read(
        struct m6_engine* engine,
        m6_word_t segment, m6_word_t offset) {

    return engine->pmem[((segment << 4) + offset) % M6_PMEM_SIZE];
}

static uint8_t m6_engine_register_segmented_read(
        struct m6_engine* engine,
        enum m6_segment_register_descriminator segment, m6_word_t offset) {

    m6_word_t base = engine->segment_registers.registers[segment];
    return m6_engine_segmented_read(engine, base, offset);
}

static void m6_engine_process_top_level(struct m6_engine* engine) {
    uint8_t byte = m6_engine_register_segmented_read(engine, M6_CS, engine->ip);

    uint8_t top = (byte & 0xF0) >> 4;
    uint8_t middle = (byte & 0xC) >> 2;
    uint8_t bottom = byte & 0x3;
    (void) bottom;

    if(top <= 0xF) {
        if(!(middle & 1)) {

        }
    }
}

void m6_engine_tick(struct m6_engine* engine) {
    m6_engine_process_top_level(engine);
}
