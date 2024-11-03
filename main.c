#include <stdio.h>
#include <stdlib.h>

#include "memory_manager.h"
#include "load_commands.h"
#include "metadata.h"
#include "exec_command.h"

int main(int ac, char** av) {
    Metadata metadata;
    MemoryQueueHead *garbaje_collector_data = setup_memory_queue();
    metadata.garbaje_collector_data = garbaje_collector_data;

    Command *commands = load_commands(metadata, ac, av);

    printf("Commands to be runned...\n");
    Command *copy = commands;
    for (; copy; copy = copy->next)
        printf("%s\n", copy->content);

    exec_command(metadata, commands);

    safe_free(metadata.garbaje_collector_data);
    return (0);
}
