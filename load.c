#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sotest.h"
#include "memory_manager.h"
#include "metadata.h"

Command *create_command_node(Metadata metadata, Command *current_command, char *content) {
    Command *new_command = (Command *) safe_malloc(metadata.garbaje_collector_data, sizeof(Command));
    new_command->content = content;
    new_command->next = NULL;

    if (current_command) {
        current_command->next = new_command;
    }

    return (new_command);
}

ssize_t get_commands(Metadata metadata, Command **commands, char *filename) {
    char *READ_MODE = "r";
    FILE *fd = fopen(filename, READ_MODE);
    if (!fd) {
        perror("File can not be oppened!");
        return (-1);
    }

    char *line = NULL;
    size_t size = 0;
    ssize_t i = 0;

    char head_has_been_set = 0;
    Command *curr = NULL;

    while(getline(&line, &size, fd) != -1) {
        char *content = (char *) safe_malloc(metadata.garbaje_collector_data, strlen(line));
        if (!content) {
            perror("Allocating memory for content has failed!");
            return (-1);
        }
        memcpy(content, line, strlen(line) - 1);
        // replacing new line with null terminatar
        content[strlen(line) - 1] = '\0';
        i++;

        curr = create_command_node(metadata, curr, content);
        if (!curr) {
            perror("Error creating command node!");
            return (-1);
        }
        if (!head_has_been_set) {
            *commands = curr;
            head_has_been_set = 1;
        }
    }
    fclose(fd);
    return (i);
}

int GENERAL_ERROR = 1;
int MISUSE_ERROR = 2;

Command *load_commands(Metadata metadata, int ac, char **av) {
    if (ac != 2) {
        perror("Only one file should be given!");
        exit(MISUSE_ERROR);
    }

    printf("Running reading file...\n");
    Command *commands = NULL;
    if (get_commands(metadata, &commands, av[1]) == -1) {
        perror("There was an error getting comands!");
        exit(MISUSE_ERROR);
    }

    return commands;
}
