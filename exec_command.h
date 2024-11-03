#ifndef EXEC_COMMAND_H
#define EXEC_COMMAND_H

#include "load_commands.h"

Command *run_call_command(Metadata metadata, Command *use_command, Command *call_command, unsigned int *line_number);

#endif
