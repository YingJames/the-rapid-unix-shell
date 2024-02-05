#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "utils.h"
#include "builtin_commands.h"

void pathCmd(char **pathv, size_t argc, char **argv) {
    if (argc == 1) {
        free(pathv[0]);
        pathv[0] = NULL;
    } else {
        size_t argIndex = 0;
        for (argIndex = 1; argv[argIndex] != NULL; argIndex++) {
            pathv[argIndex-1] = realloc(pathv[argIndex-1], (strlen(argv[argIndex]) + 1) * sizeof(char));
            strcpy(pathv[argIndex-1], argv[argIndex]);
        }
        pathv[argIndex-1] = NULL;
    }
}

void cdCmd(size_t argc, char **argv) {
    if (argc == 2) {
        if (chdir(argv[1]) != 0) {
            handleError();
        }
    } else if (argc == 1 || argc > 2) {
        handleError();
    }
}

void exitCmd(size_t argc) {
    if (argc == 1) {
        exit(0);
    } else {
        handleError();
    }
}