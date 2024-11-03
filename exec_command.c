#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <dlfcn.h>

#include "load_commands.h"
#include "parse_command.h"

int validate_library(Command *use_command) {
    char resolved_path[PATH_MAX];
    char *library_path = use_command->content + 4;

    // TODO resolve memory overflow (POSIX.1-2001)
    if (realpath(library_path, resolved_path) == NULL) {
        fprintf(stderr, "Invalid path: %s\n", library_path);
        return (0);
    }

    if (access(resolved_path, R_OK) == -1) {
        fprintf(stderr, "No read permission: %s\n", resolved_path);
        return (0);
    }

    use_command->argument = resolved_path;
    return (1);
}


Command *run_call_command(Metadata metadata, Command *use_command, Command *call_command, unsigned int *line_number) {
    Command *current_call_command = call_command;
    printf("Running %s func fron %s lib]\n", call_command->content, use_command->content);

    if (!validate_library(use_command)) {
        return current_call_command;
    }
    dlerror();

    void *handle = dlopen(use_command->argument, RTLD_LAZY);
    if (handle == NULL) {
        const char *error = dlerror();
        fprintf(stderr, "dlopen failed: %s\n", error ? error : "Unknown error");
        return current_call_command;
    }

    printf("Successfully loaded: %s\n", use_command->argument);
    void (*func)(void);
    do {
        *(void **) (&func) = dlsym(handle, current_call_command->content);
        func();
        current_call_command = lookup_call_command(metadata, use_command->next, line_number);
    } while (!current_call_command || strcmp(current_call_command->identifier, "call") != 0);

    if (dlclose(handle) != 0) {
        const char *error = dlerror();
        fprintf(stderr, "dlclose failed: %s\n", error ? error : "Unknown error");
    }

    return current_call_command;
}
