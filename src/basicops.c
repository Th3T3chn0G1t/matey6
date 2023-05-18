// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+matey6@pm.me>

#include "m6/basicops.h"
#include "m6/engine.h"

const m6_basic_op_t m6_basic_ops_table[M6_BASIC_OPS_COUNT] = {
        [M6_ADD] = m6_basic_op_add,
        [M6_OR] = m6_basic_op_or,
        [M6_ADC] = m6_basic_op_adc,
        [M6_SBB] = m6_basic_op_sbb,
        [M6_AND] = m6_basic_op_and,
        [M6_SUB] = m6_basic_op_sub,
        [M6_XOR] = m6_basic_op_xor,
        [M6_CMP] = m6_basic_op_cmp
};

m6_word_t m6_basic_op_add(
        struct m6_engine* engine, m6_word_t a, m6_word_t b) {

    (void) engine, (void) a, (void) b;
    return 0;
}

m6_word_t m6_basic_op_or(
        struct m6_engine* engine, m6_word_t a, m6_word_t b) {

    (void) engine, (void) a, (void) b;
    return 0;
}

m6_word_t m6_basic_op_adc(
        struct m6_engine* engine, m6_word_t a, m6_word_t b) {

    (void) engine, (void) a, (void) b;
    return 0;
}

m6_word_t m6_basic_op_sbb(
        struct m6_engine* engine, m6_word_t a, m6_word_t b) {

    (void) engine, (void) a, (void) b;
    return 0;
}

m6_word_t m6_basic_op_and(
        struct m6_engine* engine, m6_word_t a, m6_word_t b) {

    (void) engine, (void) a, (void) b;
    return 0;
}

m6_word_t m6_basic_op_sub(
        struct m6_engine* engine, m6_word_t a, m6_word_t b) {

    (void) engine, (void) a, (void) b;
    return 0;
}

m6_word_t m6_basic_op_xor(
        struct m6_engine* engine, m6_word_t a, m6_word_t b) {

    (void) engine, (void) a, (void) b;
    return 0;
}

m6_word_t m6_basic_op_cmp(
        struct m6_engine* engine, m6_word_t a, m6_word_t b) {

    (void) engine, (void) a, (void) b;
    return 0;
}
