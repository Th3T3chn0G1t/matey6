/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+matey6@pm.me>
 */

#include "m6/arguments.h"

static void m6_print_usage(void) {
    puts("Usage: matey6 [options]");
    puts("Options:");
    puts("-h\tShow this message");
    puts("-V\tShow version information");
    puts("-v\tUse verbose output");
    puts("");
    puts("-r file\tSpecify binary image");
    puts("-z\tZero out memory space");
	puts("-D file\tDump memory image on shutdown");
	puts("-E addr\tOverwrite reset vector address 0xFFFE/F");
    puts("-m type\tSpecify memory layout type");
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

    m6_fatal_printf("invalid memory layout type\n");
}

void m6_parse_arguments(int argc, char** argv, struct m6_opts* opts) {
    static const char options[] = "r:m:hvVzD:E:";

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
                m6_verbose = M6_TRUE;
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
                opts->zero_pmem = M6_TRUE;
                break;
            }

            case 'm': {
                opts->memory_mode = m6_process_pmem_mode(optarg);
                break;
            }

            case 'D': {
                opts->dump = optarg;
                break;
            }

			case 'E': {
				unsigned long vector = strtoul(optarg, NULL, 16);
				if(vector == ULONG_MAX) m6_fatal_errno("strtoull");
				if(vector > M6_UNSEGMENTED_MAX) {
                    m6_fatal_printf("reset vector out of acceptable range\n");
                }
				opts->overwrite_reset_vector = M6_TRUE;
				opts->reset_vector = vector;
				break;
			}
        }
    }
}
