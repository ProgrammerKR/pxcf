#ifndef PXCF_CLI_COMMANDS_H
#define PXCF_CLI_COMMANDS_H

#include <stdbool.h>

int pxcf_command_check(const char* file_path);
int pxcf_command_format(const char* file_path, bool write_in_place);
int pxcf_command_inspect(const char* file_path);

#endif // PXCF_CLI_COMMANDS_H
