#include <regex.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "metadata.h"
#include "load_commands.h"
#include "memory_manager.h"

int valid_command(Metadata metadata, const char *str, const char *pattern) {
    regex_t regex;
    int ret;
    char msgbuf[128];

    ret = regcomp(&regex, pattern, REG_EXTENDED);
    if (ret) {
        perror("Regex compilation failed!");
        safe_exit(1, metadata.garbaje_collector_data);
    }

    ret = regexec(&regex, str, 0, NULL, 0);
    if (!ret) {
        regfree(&regex);
        return (1);
    }
    else if (ret == REG_NOMATCH) {
        regfree(&regex);
        return (0);
    }
    else {
        regerror(ret, &regex, msgbuf, sizeof(msgbuf));
        fprintf(stderr, "Regex match failed: %s\n", msgbuf);
        safe_exit(1, metadata.garbaje_collector_data);
        return (-1);
    }
}


int valid_use_command(Metadata metadata, const char *str) {
    // Pattern explanation:
    // ^use             - Must start with "use"
    // [ ]              - Followed by exactly one space
    // [a-zA-Z_]        - First character after space must be letter or underscore
    // [a-zA-Z0-9_]*$   - Followed by zero or more letters, numbers, or underscores
    const char *pattern = "^use [a-zA-Z_][a-zA-Z0-9_]*$";
    return valid_command(metadata, str, pattern);
}

Command *lookup_use_command(Metadata metadata, Command *command) {
    Command *current = command;
    unsigned int line = 1;
    while (current) {
        if (strncmp(current->content, "use ", 4)) {
            fprintf(stderr, ">>> %s\n", current->content);
            fprintf(stderr, "Warning: To call a function, a library should be given first : line %u\n", line);
        } else if (!valid_use_command(metadata, current->content)) {
            fprintf(stderr, ">>> %s\n", current->content);
            fprintf(stderr, "Warning: Syntax error command func name should be following C guidelines : line %u\n", line);
        } else {
            break;
        }
        current = current->next;
        line++;
    }
    return current;
}

void exec_command(Metadata metadata, Command *command) {
    Command *use_comand = lookup_use_command(metadata, command);
    if (!use_comand) {
        perror("No 'use' command found!");
        return;
    }

    printf("Starting session for command: %s\n", use_comand->content);
}
