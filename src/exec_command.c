#include <stdio.h>
#include <string.h>
#include "exec_command.h"
#include "builtin_commands.h"

void execCommand(int pathc, char **pathv, int argc, char **argv) {
    if (argc == 0) {
        return;
    }

    if (strcmp(argv[0], "path") == 0) {
        path(argc, argv);
    }


}