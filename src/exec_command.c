#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "constants.h"
#include "builtin_commands.h"
#include "exec_command.h"

void execCommand(size_t *pathc, char pathv[MAX_PATHS][MAX_LINE], int argc, char argv[MAX_ARGS][MAX_LINE]) {
    if (strcmp(argv[0], "exit") == 0) {
        exitCmd(argc);
    }

    else if (strcmp(argv[0], "path") == 0) {
        pathCmd(pathc, pathv, argc, argv);
    }

    else if (strcmp(argv[0], "cd") == 0) {
        cdCmd(argc, argv);
    }

    else {
        char error_message[30] = "An error has occurred\n";
        write(STDERR_FILENO, error_message, strlen(error_message)); 
        fflush(stdout);
    }

}