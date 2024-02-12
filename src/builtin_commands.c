#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "include/utils.h"
#include "include/builtin_commands.h"

void pathCmd(char **pathv, int argc, char **argv) {
    if (argc == 1) {
        int pathc = strArrLen(pathv);
        for (int i = 0; i < pathc; i++)
            memset(pathv[i], 0, strlen(pathv[i]));
    } else {
        int argIndex = 0;
        for (argIndex = 1; argv[argIndex] != NULL; argIndex++) {
            pathv[argIndex-1] = realloc(pathv[argIndex-1], (MAX_LINE * sizeof(char)));
            strcpy(pathv[argIndex-1], argv[argIndex]);
            pathv[argIndex-1][strlen(argv[argIndex])] = '\0';
        }
//        memset(pathv[argIndex-1], 0, strlen(pathv[argIndex-1]));
        pathv[argIndex-1] = NULL;
    }
}

void cdCmd(int argc, char **argv) {
    if (argc == 2) {
        if (chdir(argv[1]) != 0) {
            handleError();
        }
    } else if (argc == 1 || argc > 2) {
        handleError();
    }
}

void exitCmd(int argc) {
    if (argc == 1) {
        exit(0);
    } else {
        handleError();
    }
}