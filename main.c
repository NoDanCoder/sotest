#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "memory_manager.h"
#include "load_commands.h"
#include "metadata.h"
#include "parse_command.h"
#include "interactive.h"

int main(int ac, char** av) {
    signal(SIGINT, SIG_IGN);

    Metadata metadata;
    MemoryQueueHead *garbaje_collector_data = setup_memory_queue();
    metadata.garbaje_collector_data = garbaje_collector_data;
    if (ac == 1) {
        run_interactive_mode(metadata);
    } else {
        Command *commands = load_commands(metadata, ac, av);
        parse_commands(metadata, commands);
    }

    safe_free(metadata.garbaje_collector_data);
    return (0);
}
