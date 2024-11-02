#include <stdio.h>
#include <stdlib.h>

#include "sotest.h"

int main(int ac, char** av) {
	Command *commands = load_commands(ac, av);
    
	printf("Commands to be runned...\n");
    Command *copy = commands;
    for(; copy; copy = copy->next)
        printf("%s\n", copy->content);

	return (0);
}

