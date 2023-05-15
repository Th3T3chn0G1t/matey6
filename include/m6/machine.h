// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+matey6@pm.me>

#ifndef M6_MACHINE_H
#define M6_MACHINE_H

#define M6_ADDRESS_WIDTH 20
#define M6_PMEM_SIZE 1 << M6_ADDRESS_WIDTH

#define M6_UNSEGMENTED_MAX 0xFFFF

#define M6_REGULAR_REGISTER_COUNT 8

typedef uint16_t m6_word_t;

enum m6_pmem_mode {
    M6_HIGHER_HALF_BINARY
};

enum m6_register_descriminator_16 {
    M6_AX,
    M6_CX,
    M6_DX,
    M6_BX,

    M6_SP,
    M6_BP,

    M6_SI,
    M6_DI,
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
    m6_word_t registers[M6_REGULAR_REGISTER_COUNT];
    struct m6_regular_registers_named_16 named;
};

#endif
