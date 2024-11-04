#include <stdio.h>
#include <stdlib.h>

#include "memory_manager.h"
#include "load_commands.h"
#include "metadata.h"
#include "parse_command.h"

int main(int ac, char** av) {
    Metadata metadata;
    MemoryQueueHead *garbaje_collector_data = setup_memory_queue();
    metadata.garbaje_collector_data = garbaje_collector_data;

    Command *commands = load_commands(metadata, ac, av);
    parse_commands(metadata, commands);

    safe_free(metadata.garbaje_collector_data);
    return (0);
}
