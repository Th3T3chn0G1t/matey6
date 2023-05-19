// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+matey6@pm.me>

#include "m6/engine.h"
#include "m6/basicops.h"

static uint8_t m6_engine_segmented_read(
        struct m6_engine* engine,
        m6_word_t base, m6_word_t offset);

void m6_engine_create(
        struct m6_engine_parameters* parameters, struct m6_engine* engine) {

    engine->pmem = malloc(M6_PMEM_SIZE);
    if(!engine->pmem) m6_fatal_errno("malloc");
    if(parameters->zero_pmem) memset(engine->pmem, 0, M6_PMEM_SIZE);

    static const size_t placements[] = {
            [M6_HIGHER_HALF_BINARY] = 0x8000
    };

    static const size_t size_constraints[] = {
            [M6_HIGHER_HALF_BINARY] = 0x8000
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
		pmem[M6_RESET_VECTOR] = parameters->reset_vector >> 8;
		pmem[M6_RESET_VECTOR + 1] = parameters->reset_vector & 0xFF;
	}

    uint8_t reset_lo = m6_engine_segmented_read(engine, 0, M6_RESET_VECTOR);
    uint8_t reset_hi = m6_engine_segmented_read(engine, 0, M6_RESET_VECTOR + 1);
	engine->ip = reset_lo | reset_hi << 8;
    printf("initial IP %x\n", engine->ip);
}

void m6_engine_destroy(struct m6_engine* engine) {
    free(engine->pmem);
}

static uint32_t m6_engine_effective_address(m6_word_t base, m6_word_t offset) {
    return ((base << 4) + offset);
}

static uint8_t m6_engine_segmented_read(
        struct m6_engine* engine,
        m6_word_t base, m6_word_t offset) {

    // TODO: There's something awry here % M6_PMEM_SIZE
    uint32_t address = m6_engine_effective_address(base, offset);
    uint8_t data = engine->pmem[address];
    printf("read %x @ %x [%x:%x]\n", data, address, base, offset);
    return data;
}

static uint8_t m6_engine_register_segmented_read(
        struct m6_engine* engine,
        enum m6_segment_register_descriminator segment, m6_word_t offset) {

    m6_word_t base = engine->segment_registers.registers[segment];
    return m6_engine_segmented_read(engine, base, offset);
}

static void m6_engine_mod_rm_pointers(
    struct m6_engine* engine,
    uint8_t mod_rm, m6_word_pointer_pair_t* pointers, m6_word_t disp) {

    (void) disp;

    struct m6_mod_rm_info mod_rm_info = *(struct m6_mod_rm_info*) &mod_rm;
    m6_word_t (*registers)[] = &engine->regular_registers.registers;
    union m6_rm rm = { .rm = mod_rm_info.rm };

    switch (mod_rm_info.mod) {
        case M6_REGISTER_ADDRESS: {

            break;
        }
        case M6_REGISTER_ADDRESS_DISP8: {

            break;
        }
        case M6_REGISTER_ADDRESS_DISP16: {

            break;
        }
        case M6_REGISTER: {
            (*pointers)[0] = &(*registers)[mod_rm_info.reg];
            (*pointers)[1] = &(*registers)[rm.register16];
            break;
        }
    }
}

static uint8_t m6_engine_do_basic_mod_rm_op(
        struct m6_engine* engine,
        bool wide, enum m6_basic_op_type op,
        uint8_t mod_rm, m6_word_t operands) {

    (void) op;
    uint8_t stride = 2;

    m6_word_pointer_pair_t pointers;
    m6_engine_mod_rm_pointers(engine, mod_rm, &pointers, operands);

    m6_word_t a = *pointers[0];
    m6_word_t b = *pointers[1];

    struct m6_mod_rm_info mod_rm_info = *(struct m6_mod_rm_info*) &mod_rm;

    if(!wide) {
        a >>= 8 * (mod_rm_info.rm >= M6_AH);
        a &= 0xFF;

        if(mod_rm_info.mod == M6_REGISTER) {
            b >>= 8 * (mod_rm_info.rm >= M6_AH);
            b &= 0xFF;
        }
    }

    // TODO: Only affect appropriate half of half operations
    *pointers[0] = m6_basic_ops_table[op](engine, a, b);

    return stride;
}

static bool m6_engine_process_top_level(struct m6_engine* engine) {
    uint8_t byte = m6_engine_register_segmented_read(engine, M6_CS, engine->ip);

    uint8_t top = (byte & 0xF0) >> 4;
    uint8_t middle = (byte & 0xC) >> 2;
    uint8_t bottom = byte & 0x3;

    uint8_t stride = 0;

    // TODO: This is just a temporary fix to end ticking and not how hlt should
    //       actually be handled
    if(byte == 0xF4) return false;

    if(top <= 0xF) {
        uint8_t mod_rm = m6_engine_register_segmented_read(
                engine, M6_CS, engine->ip + 1);

        uint8_t operand_0 = m6_engine_register_segmented_read(
                engine, M6_CS, engine->ip + 2);

        uint8_t operand_1 = m6_engine_register_segmented_read(
                engine, M6_CS, engine->ip + 3);

        uint16_t operands = operand_0 | (operand_1 << 8);

        enum m6_basic_op_type op = (top << 1) + ((middle & 2) >> 1);
        if(!(middle & 1)) {
            stride = m6_engine_do_basic_mod_rm_op(
                    engine, bottom & 1, op, mod_rm, operands);

            engine->ip += stride;
            return true;
        }
    }
    m6_fatal_printf(
            "illegal instruction %x @ %x [%x:%x]",
            byte, m6_engine_effective_address(
                    engine->segment_registers.named.cs, engine->ip),
            engine->segment_registers.named.cs, engine->ip);
}

bool m6_engine_tick(struct m6_engine* engine) {
    printf("ticking w/ IP %x\n", engine->ip);
    bool result = m6_engine_process_top_level(engine);
    printf(
            "state at end of tick:\n"
            "ip %x\n\n"
            "ax\t%x\n"
            "bx\t%x\n"
            "cx\t%x\n"
            "dx\t%x\n"

            "bp\t%x\n"
            "sp\t%x\n"
            "si\t%x\n"
            "di\t%x\n",
            engine->ip,

            engine->regular_registers.named.ax,
            engine->regular_registers.named.bx,
            engine->regular_registers.named.cx,
            engine->regular_registers.named.dx,

            engine->regular_registers.named.bp,
            engine->regular_registers.named.sp,
            engine->regular_registers.named.si,
            engine->regular_registers.named.di
    );

    return result;
}
