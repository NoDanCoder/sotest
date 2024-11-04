#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "load_commands.h"
#include "memory_manager.h"
#include "metadata.h"

Command *create_command_node(Metadata metadata, Command *current_command, char *content, unsigned int index) {
    Command *new_command = (Command *) safe_malloc(metadata.garbaje_collector_data, sizeof(Command));
    new_command->content = content;
    new_command->next = NULL;
    new_command->index = index;

    if (current_command) {
        current_command->next = new_command;
    }

    return (new_command);
}

ssize_t getline_2001(Metadata metadata, char **lineptr, size_t *n, FILE *stream) {
    if (lineptr == NULL || n == NULL || stream == NULL) {
        return (-1);
    }

    if (!*lineptr || *n == 0) {
        *n = 128;
        *lineptr = malloc(*n);
        if (!*lineptr) {
            perror("Error allocating memory for initial buffer (getline_2001)");
            safe_exit(1, metadata.garbaje_collector_data);
        }
    }

    char *pos = *lineptr;
    size_t bytes_read = 0;
    int c;
    while ((c = fgetc(stream)) != EOF) {
        if (bytes_read + 1 >= *n) {
            size_t new_size = *n * 2;
            if (SIZE_MAX/2 <= *n) {
                return (-1);
            }

            char *new_ptr = realloc(*lineptr, new_size);
            if (new_ptr == NULL) {
                perror("Error reallocating memory for buffer (getline_2001)");
                free(lineptr);
                safe_exit(1, metadata.garbaje_collector_data);
            }

            *lineptr = new_ptr;
            *n = new_size;
            pos = *lineptr + bytes_read;
        }

        if (ferror(stream)) {
            perror("Error reading file (getline_2001)");
            free(lineptr);
            safe_exit(1, metadata.garbaje_collector_data);
        }

        *pos++ = (char)c;
        bytes_read++;

        if (c == '\n') {
            break;
        }
    }

    if (bytes_read == 0) {
        return (-1);
    }

    *pos = '\0';
    return bytes_read;
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

    while (getline_2001(metadata, &line, &size, fd) != -1) {
        char *content = (char *) safe_malloc(metadata.garbaje_collector_data, strlen(line));
        if (!content) {
            perror("Allocating memory for content has failed!");
            return (-1);
        }
        memcpy(content, line, strlen(line) - 1);
        // replacing new line with null terminatar
        content[strlen(line) - 1] = '\0';
        i++;

        curr = create_command_node(metadata, curr, content, i);
        if (!curr) {
            perror("Error creating command node!");
            return (-1);
        }
        if (!head_has_been_set) {
            *commands = curr;
            head_has_been_set = 1;
        }
    }
    free(line);
    fclose(fd);
    return (i);
}

int GENERAL_ERROR = 1;
int MISUSE_ERROR = 2;

Command *load_commands(Metadata metadata, int ac, char **av) {
    if (ac != 2) {
        perror("Only one file should be given!");
        safe_exit(MISUSE_ERROR, metadata.garbaje_collector_data);
    }

    printf("Running reading file...\n");
    Command *commands = NULL;
    if (get_commands(metadata, &commands, av[1]) == -1) {
        perror("There was an error getting comands!");
        safe_exit(GENERAL_ERROR, metadata.garbaje_collector_data);
    }

    return commands;
}
