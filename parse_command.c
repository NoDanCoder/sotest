#include <regex.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "metadata.h"
#include "load_commands.h"
#include "memory_manager.h"
#include "exec_command.h"

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

int valid_call_command(Metadata metadata, const char *str) {
    // Pattern explanation:
    // ^call            - Must start with "call"
    // [ ]              - Followed by exactly one space
    // [a-zA-Z_]        - First character should be either alphabetic or underscore
    // [a-zA-Z0-9_]*    - Following could be digits too
    const char *pattern = "^call [a-zA-Z_][a-zA-Z0-9_]*$";
    return valid_command(metadata, str, pattern);
}

Command *lookup_use_command(Metadata metadata, Command *command) {
    Command *current = command;
    while (current) {
        if (strncmp(current->content, "call ", 5) == 0) {
            fprintf(stderr, ">>> %s\n", current->content);
            fprintf(stderr, "Warning: To call a function, a library should be given first : line %u\n", current->index);
        } else if (strncmp(current->content, "use ", 4) != 0) {
            fprintf(stderr, ">>> %s\n", current->content);
            fprintf(stderr, "Warning: ´use' and 'call' are the only active commands right now : line %u\n", current->index);
        } else if (current->content[4] == ' '){
            fprintf(stderr, ">>> %s\n", current->content);
            fprintf(stderr, "Warning: Only one space between command and argument : line %u\n", current->index);
        } else if (current->content[4] == '"' && current->content[strlen(current->content) - 1] != '"'){
            fprintf(stderr, ">>> %s\n", current->content);
            fprintf(stderr, "Warning: If path starts with (\") it should finish with them : line %u\n", current->index);
        } else if (!valid_use_command(metadata, current->content)) {
            fprintf(stderr, ">>> %s\n", current->content);
            fprintf(stderr, "Warning: Syntax error command file path should be following linux guidelines and be a '.so' file : line %u\n", current->index);
        } else {
            current->identifier = "use";
            current->argument = current->content + 4;
            break;
        }
        current = current->next;
    }
    return current;
}

Command *lookup_call_command(Metadata metadata, Command *command) {
    Command *current = command;
    while (current) {
        if (strncmp(current->content, "use ", 4) == 0) {
            current->identifier = "use";
            return current;
        } else if (strncmp(current->content, "call ", 5) != 0) {
            fprintf(stderr, ">>> %s\n", current->content);
            fprintf(stderr, "Warning: ´use' and 'call' are the only active commands right now : line %u\n", current->index);
        } else if (current->content[5] == ' ') {
            fprintf(stderr, ">>> %s\n", current->content);
            fprintf(stderr, "Warning: Only one space between command and argument : line %u\n", current->index);
        } else if (!valid_call_command(metadata, current->content)) {
            fprintf(stderr, ">>> %s\n", current->content);
            fprintf(stderr, "Warning: Syntax error command func name should be following C guidelines : line %u\n", current->index);
        } else {
            current->identifier = "call";
            current->argument = current->content + 5;
            break;
        }
        current = current->next;
    }
    return current;
}

void parse_commands(Metadata metadata, Command *command) {
    Command *use_command = NULL;
    Command *call_command = command;
    while (1) {
        use_command = lookup_use_command(metadata, call_command);
        if (!use_command) {
            perror("No 'use' command found!");
            break;
        }
        printf("Starting session for command: [%s] at line [%u]\n", use_command->content, use_command->index);

        call_command = lookup_call_command(metadata, use_command->next);
        if (!call_command || strcmp(call_command->identifier, "call") != 0) {
            perror("No 'call' command for current library found! Looking up for next use...");
        } else {
            call_command = run_call_command(metadata, use_command, call_command);
            if (!call_command) {
                printf("Program finished successfully!\n");
                break;
            }
        }
    }
    return;
}
