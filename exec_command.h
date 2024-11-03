#ifndef EXEC_COMMAND_H
#define EXEC_COMMAND_H

#include "metadata.h"
#include "load_commands.h"

int valid_command(Metadata metadata, const char *str, const char *pattern);
int valid_use_command(Metadata metadata, const char *str);
Command *lookup_use_command(Metadata metadata, Command *command);
void exec_command(Metadata metadata, Command *command);

#endif
