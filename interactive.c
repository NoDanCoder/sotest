#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "metadata.h"
#include "parse_command.h"
#include "load_commands.h"
#include "exec_command.h"

void print_ps1(void)
{
    printf("(sc) ");
    fflush(stdout);
}

void copy_to_command(Metadata metadata, const char *line, Command *command) {
    int len_line = strlen(line);
    command->content = (char *) safe_malloc(metadata.garbaje_collector_data, len_line);
    if (!command->content) {
        perror("Allocating memory for content has failed!");
        safe_exit(-1, metadata.garbaje_collector_data);
    }
    memcpy(command->content, line, len_line - 1);
    command->content[len_line - 1] = '\0';
}

void clean_command(Command *command) {
    command->content = NULL;
    command->identifier = NULL;
    command->argument = NULL;
    command->next = NULL;
}

void transfer_command(Command *self, Command *other) {
    other->content = self->content;
    other->identifier = self->identifier;
    other->argument = self->argument;
    other->next = self->next;
    clean_command(self);
}

void run_interactive_mode(Metadata metadata) {
    printf("Welcome to the interactive shell!\n");
    printf("Type 'help' for available commands\n\n");

    Command *use_command = create_command_node(metadata, NULL, NULL, 0);
    Command *call_command = create_command_node(metadata, NULL, NULL, 0);
    char *line = NULL;
    size_t size = 0;
    char session_started = 0;
    while (1) {
        print_ps1();
        if (getline_2001(metadata, &line, &size, stdin) == -1) {
            printf("\nReceived EOF. Exiting...\n");
            break;
        }
        if (!session_started) {
            copy_to_command(metadata, line, use_command);
            if (lookup_use_command(metadata, use_command) && validate_library(use_command)) {
                session_started = 1;
            }
        } else {
            copy_to_command(metadata, line, call_command);
            Command *found = lookup_call_command(metadata, call_command);
            if (found && strcmp(found->identifier, "call") == 0) {
                run_call_command(metadata, use_command, call_command);
            } else if (found && strcmp(found->identifier, "use") == 0) {
                transfer_command(call_command, use_command);
                if (!lookup_use_command(metadata, use_command)) {
                    session_started = 0;
                    clean_command(use_command);
                }
            }
        }
        printf("\n");
    }

    free(line);
}
