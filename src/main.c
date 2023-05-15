// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+matey6@pm.me>

#include "m6/arguments.h"

int main(int argc, char** argv) {
    struct m6_opts opts;
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

    free(buffer);
}
