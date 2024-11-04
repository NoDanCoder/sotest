#include <stdio.h>

void func1(void) {
    for (int i = 1; i > -10; i--)
        printf("%i", 0/i);
    printf("This is func1() from other.so\n");
}

void fail_at_runtime(void) {
    int *ptr = NULL;  // Initialize a NULL pointer
    *ptr = 42;        // Dereference NULL pointer, causing segmentation fault
}
