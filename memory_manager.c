#include <stdio.h>
#include <stdlib.h>

#include "memory_manager.h"

MemoryQueueHead *setup_memory_queue(void) {
    MemoryQueueHead *head = (MemoryQueueHead *) malloc(sizeof(MemoryQueueHead));
    head->start = NULL;
    head->end = NULL;
    return head;
}

ssize_t add_to_memory_queue(MemoryQueueHead *head, void *memory_adress) {
    MemoryQueue * node = (MemoryQueue *) malloc(sizeof(MemoryQueue));
    if (!node) {
        perror("Error allocationg memory for memory queue!");
        free(memory_adress);
        return (-1);
    }
    node->MemoryAdress = memory_adress;
    node->next = NULL;
    if (!head->start) {
        head->start = node;
    } else {
        head->end->next = node;
    }
    head->end = node;
    return 0;
}

void *safe_malloc(MemoryQueueHead *head, unsigned long bytes) {
	void *location = (void *) malloc(bytes);
	if (!location) {
		perror("Error a|llocationg safe memory!");
		return (NULL);
	}
	if (add_to_memory_queue(head, location) == -1) {
		return (NULL);
	}
	return location;
}

void safe_free(MemoryQueueHead *head) {
    MemoryQueue *current = head->start;
    MemoryQueue *next;
    while (current) {
        next = current->next;
        free(current->MemoryAdress);
        free(current);
        current = next;
    }
    free(head);
}

void safe_exit(int value, MemoryQueueHead *head) {
    safe_free(head);
    exit(value);
}
