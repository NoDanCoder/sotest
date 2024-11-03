#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <dlfcn.h>

#include "load_commands.h"
#include "parse_command.h"

int validate_library(Command *use_command) {
    char *library_path = use_command->content + 4;

    if (access(library_path, R_OK) == -1) {
        fprintf(stderr, "No read permission: %s\n", library_path);
        return (0);
    }

    use_command->argument = library_path;
    return (1);
}


Command *run_call_command(Metadata metadata, Command *use_command, Command *call_command) {
    Command *current_call_command = call_command;

    if (!validate_library(use_command)) {
        return current_call_command;
    }
    dlerror();

    void *handle = dlopen(use_command->content + 4, RTLD_LAZY);
    if (handle == NULL) {
        const char *error = dlerror();
        fprintf(stderr, "dlopen failed: %s\n", error ? error : "Unknown error");
        return current_call_command;
    }

    printf("Successfully loaded: %s\n", use_command->argument);
    void (*func)(void);
    do {
        *(void **) (&func) = dlsym(handle, current_call_command->content + 5);
        if (func) {
            func();
        } else {
            fprintf(stderr, "Method [%s] is not found! : line %u \n", current_call_command->content, current_call_command->index);
        }
        current_call_command = lookup_call_command(metadata, current_call_command->next);
    } while (current_call_command && strcmp(current_call_command->identifier, "call") == 0);

    if (dlclose(handle) != 0) {
        const char *error = dlerror();
        fprintf(stderr, "dlclose failed: %s\n", error ? error : "Unknown error");
    }

    printf("Reacher here %p\n", (void *) current_call_command);

    return current_call_command;
}
