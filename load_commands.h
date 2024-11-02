#ifndef LOAD_COMMANDS_H
#define LOAD_COMMANDS_H

#include <unistd.h>

#include "memory_manager.h"
#include "metadata.h"

typedef struct Command {
    char *content;
    struct Command *next;
} Command;

ssize_t get_commands(Metadata metadata, Command **commands, char *filename);
Command *load_commands(Metadata metadata, int ac, char **av);
Command *create_command_node(Metadata metadata, Command *current_command, char *content);

#endif

