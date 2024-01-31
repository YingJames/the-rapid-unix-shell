#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "constants.h"
#include "builtin_commands.h"

void pathCmd(size_t *pathc, char pathv[MAX_PATHS][MAX_LINE], size_t argc, char argv[MAX_ARGS][MAX_LINE]) {
    if (argc == 0) {
        *pathc = 0;
    } else {
        *pathc = argc - 1;
        for (size_t i = 1; i < argc; i++) {
            strcpy(pathv[i-1], argv[i]);
        }
    }
}

void cdCmd(size_t argc, char argv[MAX_ARGS][MAX_LINE]) {
    if (argc == 2) {
        if (chdir(argv[1]) != 0) {
            char error_message[30] = "An error has occurred\n";
            write(STDERR_FILENO, error_message, strlen(error_message)); 
            fflush(stdout);
        }
    } else if (argc == 1 || argc > 2) {
        char error_message[30] = "An error has occurred\n";
        write(STDERR_FILENO, error_message, strlen(error_message)); 
        fflush(stdout);
    }
}

void exitCmd(size_t argc) {
    if (argc == 1) {
        exit(0);
    } else {
        char error_message[30] = "An error has occurred\n";
        write(STDERR_FILENO, error_message, strlen(error_message)); 
        fflush(stdout);
    }
}