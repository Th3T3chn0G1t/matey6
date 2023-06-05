/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+matey6@pm.me>
 */

#include "m6/engine.h"
#include "m6/basicops.h"

static uint8_t m6_engine_segmented_read(
        struct m6_engine* engine,
        m6_word_t base, m6_word_t offset);

void m6_engine_create(
        struct m6_engine_parameters* parameters, struct m6_engine* engine) {

    static const size_t placements[] = {
            0x8000 /* M6_HIGHER_HALF_BINARY */
    };

    static const size_t size_constraints[] = {
			0x8000 /* M6_HIGHER_HALF_BINARY */
    };

    size_t size = parameters->binary_size;
    size_t constraint = size_constraints[parameters->pmem_mode];

    uint8_t* pmem;
    uint8_t* binary = parameters->binary;
    uint8_t* placement;
    size_t binary_size = parameters->binary_size;

    uint8_t reset_lo;
    uint8_t reset_hi;

    if(parameters->zero_pmem) {
        engine->pmem = (uint8_t*) calloc(M6_PMEM_SIZE, sizeof(uint8_t));
    }
    else {
        engine->pmem = (uint8_t*) malloc(M6_PMEM_SIZE);
    }

    if(!engine->pmem) {
        m6_fatal_errno(parameters->zero_pmem ? "calloc" : "malloc");
    }

    pmem = engine->pmem;
    placement = pmem + placements[parameters->pmem_mode];

    if(size > constraint) {
        m6_fatal_printf(
                "binary of size %lu exceeds memory mode maximum of %lu\n",
                size, constraint
        );
    }

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

    reset_lo = m6_engine_segmented_read(engine, 0, M6_RESET_VECTOR);
    reset_hi = m6_engine_segmented_read(engine, 0, M6_RESET_VECTOR + 1);
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

    /* TODO: There's something awry here % M6_PMEM_SIZE */
    uint32_t address = m6_engine_effective_address(base, offset);
    uint8_t data = engine->pmem[address];
    printf("read %x @ %x [%x:%x]\n", data, address, base, offset);
    return data;
}

static uint8_t m6_engine_register_segmented_read(
        struct m6_engine* engine,
        enum m6_segment_register_discriminator segment, m6_word_t offset) {

    m6_word_t base = engine->segment_registers.registers[segment];
    return m6_engine_segmented_read(engine, base, offset);
}

static uint8_t m6_engine_mod_rm_pointers(
    struct m6_engine* engine,
    uint8_t mod_rm, m6_word_pointer_pair_t* pointers, m6_word_t disp) {

    struct m6_mod_rm_info mod_rm_info = *(struct m6_mod_rm_info*) &mod_rm;
	m6_word_t (*registers)[M6_DI + 1] = &engine->regular_registers.registers;
	m6_word_t (*segment_registers)[M6_DS + 1] =
            &engine->segment_registers.registers;
    uint8_t rm = mod_rm_info.rm;

    uint8_t stride = 0;

	m6_word_t offset = 0;
	m6_word_t base = (*segment_registers)[M6_DS];
	uint32_t effective = 0;

	(*pointers)[0] = &(*registers)[mod_rm_info.reg];

    switch (mod_rm_info.mod) {
        case M6_REGISTER_ADDRESS_DISP8:
            offset += disp & 0xFF;
            stride = 1;
            M6_FALLTHROUGH;
            /* FALLTHRU */

        case M6_REGISTER_ADDRESS_DISP16:
            offset += disp;
            stride = 2;
            M6_FALLTHROUGH;
            /* FALLTHRU */

        case M6_REGISTER_ADDRESS: {
			switch((enum m6_rm_register_address) rm) {
				case M6_ADDRESS_BX_SI: {
					offset += (*registers)[M6_BX] + (*registers)[M6_SI];
					break;
				}
				case M6_ADDRESS_BX_DI: {
					offset += (*registers)[M6_BX] + (*registers)[M6_DI];
					break;
				}
				case M6_ADDRESS_BP_SI: {
					offset += (*registers)[M6_BP] + (*registers)[M6_SI];
					break;
				}
				case M6_ADDRESS_BP_DI: {
					offset += (*registers)[M6_BP] + (*registers)[M6_DI];
					break;
				}
				case M6_ADDRESS_SI: {
					offset += (*registers)[M6_SI];
					break;
				}
				case M6_ADDRESS_DI: {
					offset += (*registers)[M6_DI];
					break;
				}
				case M6_ADDRESS_DIRECT: {
                    if(
                            mod_rm_info.mod == M6_REGISTER_ADDRESS_DISP8 ||
                            mod_rm_info.mod == M6_REGISTER_ADDRESS_DISP16
                    ) {
                        offset += (*registers)[M6_BP];
                    }
                    else {
                        stride = 2;
                        offset = disp;
                    }
					break;
				}
				case M6_ADDRESS_BX: {
					offset += (*registers)[M6_BX];
					break;
				}
			}

			if(engine->segment_override != M6_SEGMENT_NONE) {
				base = (*segment_registers)[engine->segment_override];
			}

			effective = m6_engine_effective_address(base, offset);
            /*
             * TODO: Full r/w indirection
             * or DMA w/ effective address spoofing?
             */
			(*pointers)[1] = (m6_word_t*) &engine->pmem[effective];

            break;
        }
        case M6_REGISTER: {
            (*pointers)[1] = &(*registers)[rm];
            break;
        }
    }

    return stride;
}

static uint8_t m6_engine_do_basic_mod_rm_op(
        struct m6_engine* engine,
        m6_bool_t wide, enum m6_basic_op_type op,
        uint8_t mod_rm, m6_word_t operands) {

    uint8_t stride = 2;

    m6_word_pointer_pair_t pointers;
    m6_word_t a;
    m6_word_t b;

    struct m6_mod_rm_info mod_rm_info = *(struct m6_mod_rm_info*) &mod_rm;

    stride += m6_engine_mod_rm_pointers(engine, mod_rm, &pointers, operands);

    a = *pointers[0];
    b = *pointers[1];

    if(!wide) {
        a >>= 8 * (mod_rm_info.rm >= M6_AH);
        a &= 0xFF;

        if(mod_rm_info.mod == M6_REGISTER) {
            b >>= 8 * (mod_rm_info.rm >= M6_AH);
            b &= 0xFF;
        }
    }

    /* TODO: Only affect appropriate half of half operations */
    *pointers[0] = m6_basic_ops_table[op](engine, a, b);

    return stride;
}

static m6_bool_t m6_engine_process_top_level(struct m6_engine* engine) {
	m6_word_t ip = engine->ip;
    uint8_t byte = m6_engine_register_segmented_read(engine, M6_CS, ip);

    uint8_t top = (byte & 0xF0) >> 4;
    uint8_t middle = (byte & 0xC) >> 2;
    uint8_t bottom = byte & 0x3;

    uint8_t stride = 0;

    /*
     * TODO: This is just a temporary fix to end ticking and not how hlt should
     * actually be handled
     */
    if(byte == 0xF4) return M6_FALSE;

    if(top <= 0xF) {
        uint8_t mod_rm = m6_engine_register_segmented_read(
                engine, M6_CS, ip + 1);

        uint8_t operand_0 = m6_engine_register_segmented_read(
                engine, M6_CS, ip + 2);

        uint8_t operand_1 = m6_engine_register_segmented_read(
                engine, M6_CS, ip + 3);

        uint16_t operands = operand_0 | (operand_1 << 8);

        enum m6_basic_op_type op =
                (enum m6_basic_op_type) ((top << 1) + ((middle & 2) >> 1));

        if(!(middle & 1)) {
            stride = m6_engine_do_basic_mod_rm_op(
                    engine, bottom & 1, op, mod_rm, operands);

            engine->ip += stride;
            return M6_TRUE;
        }
    }
    m6_fatal_printf(
            "illegal instruction %x @ %x [%x:%x]",
            byte, m6_engine_effective_address(
                    engine->segment_registers.named.cs, engine->ip),
            engine->segment_registers.named.cs, engine->ip);
}

m6_bool_t m6_engine_tick(struct m6_engine* engine) {
    m6_bool_t result = m6_engine_process_top_level(engine);

	engine->segment_override = M6_SEGMENT_NONE;

    printf("ticking w/ IP %x\n", engine->ip);
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
