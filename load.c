#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sotest.h"


ssize_t get_commands(char **commands, char *filename) {
    char *READ_MODE = "r";
    FILE *fd = fopen(filename, READ_MODE);
    if (!fd) {
        perror("File can not be oppened!");
        return (-1);
    }

    char *line = NULL;
    size_t size = 0;
    int i = 0;
    while(getline(&line, &size, fd) != -1) {
        commands[i] = (char *) malloc(strlen(line));
        if (!commands[i]) {
            perror("Allocating memory has failed!");
            return (-1);
        }
        memcpy(commands[i], line, strlen(line) - 1);
        // replacing new line with null terminatar
        commands[i][strlen(line) - 1] = '\0';
        i++;
    }
    commands[i] = NULL;
    return (sizeof(commands));
}

