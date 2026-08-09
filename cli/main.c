#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pxcf/pxcf.h"
#include "commands.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("PXCF CLI\n");
        printf("Usage: pxcf <command> [args]\n");
        printf("Commands:\n");
        printf("  check <file>    Validate a PXCF file\n");
        printf("  format <file>   Format a PXCF file\n");
        printf("  inspect <file>  Display parsed structure\n");
        printf("  version         Show PXCF version\n");
        return 1;
    }

    const char* command = argv[1];
    
    if (strcmp(command, "version") == 0) {
        printf("PXCF version %s\n", PXCF_VERSION_STRING);
        return 0;
    } else if (strcmp(command, "check") == 0) {
        if (argc < 3) {
            fprintf(stderr, "Usage: pxcf check <file>\n");
            return 1;
        }
        return pxcf_command_check(argv[2]);
    } else if (strcmp(command, "format") == 0) {
        if (argc < 3) {
            fprintf(stderr, "Usage: pxcf format <file> [--write]\n");
            return 1;
        }
        bool write_in_place = (argc >= 4 && strcmp(argv[3], "--write") == 0);
        return pxcf_command_format(argv[2], write_in_place);
    } else if (strcmp(command, "inspect") == 0) {
        if (argc < 3) {
            fprintf(stderr, "Usage: pxcf inspect <file>\n");
            return 1;
        }
        return pxcf_command_inspect(argv[2]);
    } else if (strcmp(command, "help") == 0) {
        printf("PXCF CLI\n");
        return 0;
    } else {
        fprintf(stderr, "Unknown command: %s\n", command);
        return 1;
    }
}
