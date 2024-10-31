#include <stdio.h>
#include <stdlib.h>

int ERROR_COMMAND_NOT_FOUND = 127;

int main(int ac, char** av) {
    (void) av;
    if (ac != 2) {
        perror("Only one file should be given!");
        exit(ERROR_COMMAND_NOT_FOUND);
    }
}
