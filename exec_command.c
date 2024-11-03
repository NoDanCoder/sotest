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
    // (/|\\./|~/)?     - Could start with "/", "./", "~/" or nothing
    // (.+/)*           - Match all folder that could have path
    // [^/]+\\.so$.     - File sould start with at least one character before ending with .so extention
    const char *pattern = "^use [\"\']?(/|\\./|~/)?(.+/)*[^/]+\\.so[\"\']?$";
    return valid_command(metadata, str, pattern);
}

Command *lookup_use_command(Metadata metadata, Command *command, unsigned int *line_number) {
    Command *current = command;
    while (current) {
        if (strncmp(current->content, "call ", 5) == 0) {
            fprintf(stderr, ">>> %s\n", current->content);
            fprintf(stderr, "Warning: To call a function, a library should be given first : line %u\n", *line_number);
        } else if (strncmp(current->content, "use ", 4) != 0) {
            fprintf(stderr, ">>> %s\n", current->content);
            fprintf(stderr, "Warning: ´use' and 'call' are the only active commands right now : line %u\n", *line_number);
        } else if(current->content[4] == ' '){
            fprintf(stderr, ">>> %s\n", current->content);
            fprintf(stderr, "Warning: Only one space between command and argument : line %u\n", *line_number);
        } else if(current->content[4] == '"' && current->content[strlen(current->content) - 1] != '"'){
            fprintf(stderr, ">>> %s\n", current->content);
            fprintf(stderr, "Warning: If path starts with (\") it should finish with them : line %u\n", *line_number);
        } else if (!valid_use_command(metadata, current->content)) {
            fprintf(stderr, ">>> %s\n", current->content);
            fprintf(stderr, "Warning: Syntax error command file path should be following linux guidelines : line %u\n", *line_number);
        } else {
            break;
        }
        current = current->next;
        *line_number += 1;
    }
    return current;
}

void exec_command(Metadata metadata, Command *command) {
    unsigned int line_number = 1;
    Command *use_comand = lookup_use_command(metadata, command, &line_number);
    if (!use_comand) {
        perror("No 'use' command found!");
        return;
    }

    printf("Starting session for command: [%s] at line [%u]\n", use_comand->content, line_number);
}
