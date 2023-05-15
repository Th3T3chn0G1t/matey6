// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+matey6@pm.me>

#include "m6/arguments.h"
#include "m6/engine.h"

int main(int argc, char** argv) {
    struct m6_opts opts = { 0 };
    m6_parse_arguments(argc, argv, &opts);

    FILE* f = fopen(opts.binary, "r");
    if(!f) m6_fatal_errno("fopen");

    int result = fseek(f, 0, SEEK_END);
    if(result == -1) m6_fatal_errno("fseek");

    long pos = ftell(f);
    if(pos == -1) m6_fatal_errno("ftell");
    rewind(f);

    uint8_t* buffer = malloc(pos);
    if(!buffer) m6_fatal_errno("malloc");

    size_t n = fread(buffer, sizeof(uint8_t), pos, f);
    if(n == 0) {
        result = ferror(f);
        if(result) m6_fatal_errno("fread");
    }

    result = fclose(f);
    if(result == EOF) m6_fatal_errno("fclose");

    struct m6_engine_parameters parameters = {
            .pmem_mode = opts.memory_mode,
            .binary = buffer,
            .binary_size = pos,
            .zero_pmem = opts.zero_pmem
    };
    struct m6_engine engine = { 0 };
    m6_engine_create(&parameters, &engine);

    fwrite(engine.pmem, 1, M6_PMEM_SIZE, fopen("test.bin", "w+"));

    m6_engine_destroy(&engine);
    free(buffer);
}
