/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+matey6@pm.me>
 */

#include "m6/basicops.h"
#include "m6/engine.h"

const m6_basic_op_t m6_basic_ops_table[] = {
        m6_basic_op_add,
        m6_basic_op_or,
        m6_basic_op_adc,
        m6_basic_op_sbb,
        m6_basic_op_and,
        m6_basic_op_sub,
        m6_basic_op_xor,
        m6_basic_op_cmp
};

m6_word_t m6_basic_op_add(
        struct m6_engine* engine, m6_word_t a, m6_word_t b) {

    m6_word_t result = 0;
    result = a + b;

    /* TODO: Flags
     * Overflow
     * Sign
     * Auxiliary Carry
     * Parity
     * Carry
     */

    if(!result) engine->flags.zero = M6_TRUE;
    return result;
}

m6_word_t m6_basic_op_or(
        struct m6_engine* engine, m6_word_t a, m6_word_t b) {

    m6_word_t result = 0;

    engine->flags.overflow = M6_FALSE;
    engine->flags.carry = M6_FALSE;

    result = a | b;

    /* TODO: Flags
     * Sign
     * Auxiliary Carry -> Undefined (Replicate 8086 behaviours?)
     * Parity
     */

    if(!result) engine->flags.zero = M6_TRUE;
    return result;
}

m6_word_t m6_basic_op_adc(
        struct m6_engine* engine, m6_word_t a, m6_word_t b) {

    m6_word_t result = 0;
    result = a + b; /* TODO: Carry? */

    /* TODO: Flags
     * Overflow
     * Sign
     * Auxiliary Carry
     * Parity
     * Carry
     */

    if(!result) engine->flags.zero = M6_TRUE;
    return result;
}

m6_word_t m6_basic_op_sbb(
        struct m6_engine* engine, m6_word_t a, m6_word_t b) {

    m6_word_t result = 0;
    result = a - b; /* TODO: Borrow? */

    /* TODO: Flags
     * Overflow
     * Sign
     * Auxiliary Carry
     * Parity
     * Carry
     */

    if(!result) engine->flags.zero = M6_TRUE;
    return result;
}

m6_word_t m6_basic_op_and(
        struct m6_engine* engine, m6_word_t a, m6_word_t b) {

    m6_word_t result = 0;

    engine->flags.overflow = M6_FALSE;
    engine->flags.carry = M6_FALSE;

    result = a & b;

    /* TODO: Flags
     * Sign
     * Auxiliary Carry -> Undefined (Replicate 8086 behaviours?)
     * Parity
     */

    if(!result) engine->flags.zero = M6_TRUE;
    return result;
}

m6_word_t m6_basic_op_sub(
        struct m6_engine* engine, m6_word_t a, m6_word_t b) {

    m6_word_t result = 0;
    result = a - b;

    /* TODO: Flags
     * Overflow
     * Sign
     * Auxiliary Carry
     * Parity
     * Carry
     */

    if(!result) engine->flags.zero = M6_TRUE;
    return result;
}

m6_word_t m6_basic_op_xor(
        struct m6_engine* engine, m6_word_t a, m6_word_t b) {

    m6_word_t result = 0;

    engine->flags.overflow = M6_FALSE;
    engine->flags.carry = M6_FALSE;

    result = a ^ b;

    /* TODO: Flags
     * Sign
     * Auxiliary Carry -> Undefined (Replicate 8086 behaviours?)
     * Parity
     */

    if(!result) engine->flags.zero = M6_TRUE;
    return result;
}

m6_word_t m6_basic_op_cmp(
        struct m6_engine* engine, m6_word_t a, m6_word_t b) {

    m6_word_t result = 0;
    result = a - b;

    /* TODO: Flags
     * Overflow
     * Sign
     * Auxiliary Carry
     * Parity
     * Carry
     */

    if(!result) engine->flags.zero = M6_TRUE;
    return a; /* cmp does not affect its changes on machine state */
}
