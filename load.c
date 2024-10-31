#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sotest.h"

Command *create_command_node(Command *current_command, char *content) {
	Command *new_command = (Command *) malloc(sizeof(Command));
	new_command->content = content;
	new_command->next = NULL;

	if (current_command) {
		current_command->next = new_command;
	}

	return (new_command);
}

ssize_t get_commands(Command **commands, char *filename) {
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
        char *content = (char *) malloc(strlen(line));
        if (!content) {
            perror("Allocating memory for content has failed!");
            return (-1);
        }
        memcpy(content, line, strlen(line) - 1);
        // replacing new line with null terminatar
        content[strlen(line) - 1] = '\0';
        i++;

		curr = create_command_node(curr, content);
		if (!curr) {
			perror("Error creating command node!");
			return (-1);
		}
		if (!head_has_been_set) {
			*commands = curr;
			head_has_been_set = 1;
		}
    }
    return (i);
}

