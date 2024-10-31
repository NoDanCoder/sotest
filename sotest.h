#ifndef SOTEST_H
#define SOTEST_H

#include <unistd.h>

typedef struct Command {
	char *content;
	struct Command *next;
} Command;

ssize_t get_commands(Command **commands, char *filename);

#endif

