// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+matey6@pm.me>

#include "m6/arguments.h"

static void m6_print_usage(void) {
    puts("Usage: matey6 [options]");
    puts("Options:");
    puts("-h\tShow this message");
    puts("-V\tShow version information");
    puts("-v\tUse verbose output");
    puts("");
    puts("-r\tSpecify binary image");
    puts("-z\tZero out memory space");
    puts("-m\tSpecify memory layout type");
    puts(
            "\thigherhalf"
            "\tPlace binary image in the higher half of the\n\t"
            "\t\tunsegmented address space"
    );
}

static void m6_print_version(void) {
    puts("matey6 " M6_VERSION "\n");
    puts(
            "Copyright (C) 2023 Emily \"TTG\" Banerjee "
            "<prs.ttg+matey6@pm.me>"
    );
    puts(
            "This is free software; "
            "see the source for copying conditions."
    );
    puts(
            "There is NO warranty; "
            "not even for MERCHANTABILITY or FITNESS FOR A\n"
            "PARTICULAR PURPOSE.\n"
    );
}

static enum m6_pmem_mode m6_process_pmem_mode(const char* mode_str) {
    if(!strcmp(mode_str, "higherhalf")) return M6_HIGHER_HALF_BINARY;

    m6_fatal_printf("invalid memory layout type\n", stderr);
}

void m6_parse_arguments(int argc, char** argv, struct m6_opts* opts) {
    static const char options[] = "r:m:hvVz";

    int result = 0;

    while(result != -1) {
        result = getopt(argc, argv, options);
        switch(result) {
            case '?': {
                m6_print_usage();
                exit(EXIT_FAILURE);
            }

            case 'h': {
                m6_print_usage();
                exit(EXIT_SUCCESS);
            }

            case 'v': {
                m6_verbose = true;
                break;
            }

            case 'V': {
                m6_print_version();
                exit(EXIT_SUCCESS);
            }

            case 'r': {
                opts->binary = optarg;
                break;
            }

            case 'z': {
                opts->zero_pmem = true;
                break;
            }

            case 'm': {
                opts->memory_mode = m6_process_pmem_mode(optarg);
                break;
            }
        }
    }
}
