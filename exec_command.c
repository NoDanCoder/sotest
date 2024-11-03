#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <dlfcn.h>
#include <sys/wait.h>

#include "load_commands.h"
#include "parse_command.h"

int validate_library(Command *use_command) {
    if (access(use_command->argument, R_OK) == -1) {
        fprintf(stderr, "No read permission: %s\n", use_command->argument);
        return (0);
    }

    return (1);
}

void run_func(void (*func)(void)) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Error trying to fork process for library call");
    } else if (pid == 0) {
        func();
        exit(0);
    } else {
        int status;
        wait(&status);
        printf("Finished process of running library with status %d\n\n", status);
    }
}


Command *run_call_command(Metadata metadata, Command *use_command, Command *call_command) {
    Command *current_call_command = call_command;

    if (!validate_library(use_command)) {
        return current_call_command;
    }

    void *handle = dlopen(use_command->argument, RTLD_LAZY);
    if (handle == NULL) {
        const char *error = dlerror();
        fprintf(stderr, "dlopen failed: %s\n", error ? error : "Unknown error");
        return current_call_command;
    }

    printf("Successfully loaded: %s\n", use_command->argument);
    void (*func)(void);
    do {
        *(void **) (&func) = dlsym(handle, current_call_command->argument);
        if (func) {
            printf("Running method [%s] from lib [%s]:\n\n", current_call_command->argument, use_command->argument);
            run_func(func);
        } else {
            const char *error = dlerror();
            fprintf(stderr, "Get function from lib failed: %s\n", error ? error : "Unknown error");
            fprintf(stderr, "Method [%s] is not found! : line %u \n", current_call_command->content, current_call_command->index);
        }
        current_call_command = lookup_call_command(metadata, current_call_command->next);
    } while (current_call_command && strcmp(current_call_command->identifier, "call") == 0);

    if (dlclose(handle) != 0) {
        const char *error = dlerror();
        fprintf(stderr, "dlclose failed: %s\n", error ? error : "Unknown error");
    }

    return current_call_command;
}
