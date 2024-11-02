#ifndef MEMORY_TRACKER_H
#define MEMORY_TRACKER_H

#include <unistd.h>

typedef struct MemoryQueue {
    void *MemoryAdress;
    struct MemoryQueue *next;
} MemoryQueue;

typedef struct MemoryQueueHead {
    MemoryQueue *start;
    MemoryQueue *end;
} MemoryQueueHead;

MemoryQueueHead *setup_memory_queue(void);
ssize_t add_to_memory_queue(MemoryQueueHead *head, void *memory_adress);
void *safe_malloc(MemoryQueueHead *head, unsigned long bytes);
void safe_exit(int value, MemoryQueueHead *head);

#endif
