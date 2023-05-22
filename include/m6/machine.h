/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+matey6@pm.me>
 */

#ifndef M6_MACHINE_H
#define M6_MACHINE_H

#define M6_ADDRESS_WIDTH 20
#define M6_PMEM_SIZE 1 << M6_ADDRESS_WIDTH

#define M6_UNSEGMENTED_MAX 0xFFFF

#define M6_RESET_VECTOR 0xFFFE

typedef uint16_t m6_word_t;
typedef m6_word_t m6_word_pair_t[2];
typedef m6_word_t* m6_word_pointer_pair_t[2];

enum m6_pmem_mode {
    M6_HIGHER_HALF_BINARY
};

enum m6_regular_register_discriminator_16 {
    M6_AX,
    M6_CX,
    M6_DX,
    M6_BX,

    M6_SP,
    M6_BP,

    M6_SI,
    M6_DI
};

enum m6_regular_register_discriminator_8 {
    M6_AL,
    M6_CL,
    M6_DL,
    M6_BL,

    M6_AH,
    M6_CH,
    M6_DH,
    M6_BH
};

/* NOTE: This MUST reflect the order of m6_register_discriminator_16 */
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

enum m6_segment_register_discriminator {
	M6_SEGMENT_NONE,

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

enum m6_mod {
    M6_REGISTER_ADDRESS,
    M6_REGISTER_ADDRESS_DISP8,
    M6_REGISTER_ADDRESS_DISP16,
    M6_REGISTER
};

enum m6_rm_register_address {
    M6_ADDRESS_BX_SI,
    M6_ADDRESS_BX_DI,
    M6_ADDRESS_BP_SI,
    M6_ADDRESS_BP_DI,
    M6_ADDRESS_SI,
    M6_ADDRESS_DI,
    M6_ADDRESS_DIRECT,
    M6_ADDRESS_BX
};

enum m6_rm_register_address_disp8 {
    M6_ADDRESS_BX_SI_DISP8,
    M6_ADDRESS_BX_DI_DISP8,
    M6_ADDRESS_BP_SI_DISP8,
    M6_ADDRESS_BP_DI_DISP8,
    M6_ADDRESS_SI_DISP8,
    M6_ADDRESS_DI_DISP8,
    M6_ADDRESS_BP_DISP8,
    M6_ADDRESS_BX_DISP8
};

enum m6_rm_register_address_disp16 {
    M6_ADDRESS_BX_SI_DISP16,
    M6_ADDRESS_BX_DI_DISP16,
    M6_ADDRESS_BP_SI_DISP16,
    M6_ADDRESS_BP_DI_DISP16,
    M6_ADDRESS_SI_DISP16,
    M6_ADDRESS_DI_DISP16,
    M6_ADDRESS_BP_DISP16,
    M6_ADDRESS_BX_DISP16
};

struct m6_mod_rm_info {
	m6_ubitfield_t rm : 3;
	m6_ubitfield_t reg : 3;
	m6_ubitfield_t mod : 2; /* enum m6_mod */
} M6_PACKED;

struct m6_flags {
	m6_ubitfield_t carry : 1;
	m6_ubitfield_t parity : 1;
	m6_ubitfield_t auxiliary_carry : 1;
	m6_ubitfield_t zero : 1;
	m6_ubitfield_t sign : 1;
    m6_ubitfield_t overflow : 1;
    m6_ubitfield_t interrupt_enable : 1;
    m6_ubitfield_t direction : 1;
    m6_ubitfield_t trap : 1;
} M6_PACKED;

#endif
