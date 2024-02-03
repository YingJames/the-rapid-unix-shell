#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "utils.h"
#include "builtin_commands.h"

void pathCmd(char **pathv, char **argv) {
    // find argc
    size_t argc = 0;
    for (size_t i = 0; i < MAX_ARGS; i++) {
        if (argv[i] == NULL) {
            argc = i;
            break;
        }
    }

    if (argc == 1) {
        pathv[0] = realloc(pathv[0], (strlen("/bin") + 1) * sizeof(char));
        pathv[1] = NULL;
    } else {
        size_t argIndex = 0;
        for (size_t argIndex = 1; argv[argIndex] != NULL; argIndex++) {
            pathv[argIndex] = realloc(pathv[argIndex], (strlen(argv[argIndex]) + 1) * sizeof(char));
            strcpy(pathv[argIndex], argv[argIndex]);
        }
        pathv[argIndex] = NULL;
    }
}

void cdCmd(size_t argc, char argv[MAX_ARGS][MAX_LINE]) {
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