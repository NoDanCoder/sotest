#ifndef PARSE_COMMAND_H
#define PARSE_COMMAND_H

#include "metadata.h"
#include "load_commands.h"

int valid_file_name(Metadata metadata, const char *str);
int valid_command(Metadata metadata, const char *str, const char *pattern);
int valid_use_command(Metadata metadata, const char *str);
Command *lookup_use_command(Metadata metadata, Command *command);
Command *lookup_call_command(Metadata metadata, Command *command);
void parse_commands(Metadata metadata, Command *command);

#endif
