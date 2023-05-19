/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+matey6@pm.me>
 */

#ifndef M6_BASICOPS_H
#define M6_BASICOPS_H

#include "engine.h"

enum m6_basic_op_type {
    M6_ADD,
    M6_OR,
    M6_ADC,
    M6_SBB,
    M6_AND,
    M6_SUB,
    M6_XOR,
    M6_CMP
};

typedef m6_word_t (*m6_basic_op_t)(
        struct m6_engine* engine, m6_word_t a, m6_word_t b);

extern const m6_basic_op_t m6_basic_ops_table[M6_CMP + 1];

m6_word_t m6_basic_op_add(
        struct m6_engine* engine, m6_word_t a, m6_word_t b);
m6_word_t m6_basic_op_or(
        struct m6_engine* engine, m6_word_t a, m6_word_t b);
m6_word_t m6_basic_op_adc(
        struct m6_engine* engine, m6_word_t a, m6_word_t b);
m6_word_t m6_basic_op_sbb(
        struct m6_engine* engine, m6_word_t a, m6_word_t b);
m6_word_t m6_basic_op_and(
        struct m6_engine* engine, m6_word_t a, m6_word_t b);
m6_word_t m6_basic_op_sub(
        struct m6_engine* engine, m6_word_t a, m6_word_t b);
m6_word_t m6_basic_op_xor(
        struct m6_engine* engine, m6_word_t a, m6_word_t b);
m6_word_t m6_basic_op_cmp(
        struct m6_engine* engine, m6_word_t a, m6_word_t b);

#endif
