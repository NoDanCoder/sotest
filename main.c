#include <stdio.h>
#include <stdlib.h>

#include "sotest.h"

int GENERAL_ERROR = 1;
int MISUSE_ERROR = 2;


int main(int ac, char** av) {
    if (ac != 2) {
        perror("Only one file should be given!");
        exit(MISUSE_ERROR);
    }
    
    printf("Running reading file...\n");
    Command *commands = NULL;
    if (get_commands(&commands, av[1]) == -1) {
        perror("There was an error getting comands!");
        exit(MISUSE_ERROR);
    }
        
    
    printf("Commands to be runned...\n");
    Command *copy = commands;
    for(; copy; copy = copy->next)
        printf("%s\n", copy->content);

	return (0);
}

