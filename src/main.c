/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+matey6@pm.me>
 */

#include "m6/arguments.h"
#include "m6/engine.h"

int main(int argc, char** argv) {
    FILE* f;
    int result;
    long pos;
    uint8_t* buffer;
    size_t n;

    struct m6_engine_parameters parameters;
    struct m6_engine engine;

	struct m6_opts opts;

	memset(&parameters, 0, sizeof(parameters));
	memset(&engine, 0, sizeof(engine));
	memset(&opts, 0, sizeof(opts));

    m6_parse_arguments(argc, argv, &opts);

    f = fopen(opts.binary, "r");
    if(!f) m6_fatal_errno("fopen");

    result = fseek(f, 0, SEEK_END);
    if(result == -1) m6_fatal_errno("fseek");

    pos = ftell(f);
    if(pos == -1) m6_fatal_errno("ftell");
    rewind(f);

    buffer = (uint8_t*) malloc(pos);
    if(!buffer) m6_fatal_errno("malloc");

    n = fread(buffer, sizeof(uint8_t), pos, f);
    if(n == 0) {
        result = ferror(f);
        if(result) m6_fatal_errno("fread");
    }

    result = fclose(f);
    if(result == EOF) m6_fatal_errno("fclose");

    parameters.pmem_mode = opts.memory_mode;
    parameters.binary = buffer;
    parameters.binary_size = pos;
    parameters.overwrite_reset_vector = opts.overwrite_reset_vector;
    parameters.reset_vector = opts.reset_vector;
    parameters.zero_pmem = opts.zero_pmem;

    m6_engine_create(&parameters, &engine);

    while(m6_engine_tick(&engine)) continue;

    if(opts.dump) {
        size_t written;

        FILE* dump = fopen(opts.dump, "w+");
        if(!dump) m6_fatal_errno("fopen");

        written = fwrite(
                engine.pmem, sizeof(uint8_t), M6_PMEM_SIZE, dump);

        if(written < M6_PMEM_SIZE) m6_fatal_errno("fwrite");
        fclose(dump);
    }

    m6_engine_destroy(&engine);
    free(buffer);

	return EXIT_SUCCESS;
}
