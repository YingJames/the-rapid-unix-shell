#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "constants.h"
#include "builtin_commands.h"

void pathCmd(int *pathc, char pathv[MAX_PATHS][MAX_LINE], int argc, char argv[MAX_ARGS][MAX_LINE]) {
    if (argc == 0) {
        *pathc = 0;
    } else {
        *pathc = argc - 1;
        for (int i = 1; i < argc; i++) {
            strcpy(pathv[i-1], argv[i]);
        }
    }
}

void exitCmd(int argc) {
    if (argc == 1) {
        exit(0);
    } else {
        char error_message[30] = "An error has occurred\n";
        write(STDERR_FILENO, error_message, strlen(error_message)); 
        fflush(stdout);
    }
}