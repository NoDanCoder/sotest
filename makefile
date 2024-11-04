# Compiler settings
CC = gcc
CFLAGS = -D_POSIX_C_SOURCE=200112L -Wall -Werror -Wextra -pedantic

# Source files (excluding test.c and other.c)
SRC = $(filter-out test.c other.c, $(wildcard *.c))
EXEC = sotest
SO_FILES = test.so other.so

# Default target
all: $(EXEC)

# Compile .c files into executable (excluding test.c and other.c)
$(EXEC): $(SRC)
	$(CC) $(CFLAGS) $^ -o $@ -ldl

# Create shared objects
taste: $(EXEC) test.so other.so

test.so: test.c
	$(CC) -Wall -fPIC -shared -o $@ $<

other.so: other.c
	$(CC) -Wall -fPIC -shared -o $@ $<

# Clean target
clean:
	rm -f $(SO_FILES) $(EXEC)

# Declare phony targets
.PHONY: all clean taste