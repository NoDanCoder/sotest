#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "metadata.h"
#include "parse_command.h"
#include "load_commands.h"

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

void run_interactive_mode(Metadata metadata) {
    printf("Welcome to the interactive shell!\n");
    printf("Type 'help' for available commands\n\n");

    Command *command = create_command_node(metadata, NULL, NULL, 0);
    char *line = NULL;
    size_t size = 0;

    while (1) {
        print_ps1();
        if (getline_2001(metadata, &line, &size, stdin) == -1) {
            printf("\nReceived EOF. Exiting...\n");
            break;
        }
        copy_to_command(metadata, line, command);
        if (lookup_use_command(metadata, command)) {
            printf("well done!");
        }
        printf("\n");
    }

    free(line);
}
