// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+matey6@pm.me>

#ifndef M6_MACHINE_H
#define M6_MACHINE_H

#define M6_ADDRESS_WIDTH 20
#define M6_PMEM_SIZE 1 << M6_ADDRESS_WIDTH

#define M6_UNSEGMENTED_MAX 0xFFFF

#define M6_RESET_VECTOR 0xFFFE

typedef uint16_t m6_word_t;

enum m6_pmem_mode {
    M6_HIGHER_HALF_BINARY
};

enum m6_regular_register_descriminator_16 {
    M6_AX,
    M6_CX,
    M6_DX,
    M6_BX,

    M6_SP,
    M6_BP,

    M6_SI,
    M6_DI
};

// NOTE: This MUST reflect the order of m6_register_descriminator_16
struct m6_regular_registers_named_16 {
    m6_word_t ax;
    m6_word_t cx;
    m6_word_t dx;
    m6_word_t bx;

    m6_word_t sp;
    m6_word_t bp;

    m6_word_t si;
    m6_word_t di;
} M6_PACKED;

union m6_regular_registers {
	m6_word_t registers[M6_DI + 1];
	struct m6_regular_registers_named_16 named;
};

enum m6_segment_register_descriminator {
	M6_ES,
	M6_CS,
	M6_SS,
	M6_DS
};

struct m6_segment_registers_named {
	m6_word_t es;
	m6_word_t cs;
	m6_word_t ss;
	m6_word_t ds;
} M6_PACKED;

union m6_segment_registers {
	m6_word_t registers[M6_DS + 1];
	struct m6_segment_registers_named named;
};

#endif
