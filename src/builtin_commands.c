#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "include/utils.h"
#include "include/builtin_commands.h"

void pathCmd(char **pathv, size_t argc, char **argv) {
    // if the user only types "path" then clear the pathv
    if (argc == 1) {
        free(pathv[0]);
        pathv[0] = NULL;
    } else {
        // realloc memory for the new pathv
        size_t argIndex = 0;
        for (argIndex = 1; argv[argIndex] != NULL; argIndex++) {
            pathv[argIndex - 1] = realloc(pathv[argIndex - 1], (strlen(argv[argIndex]) + 1) * sizeof(char));
            strcpy(pathv[argIndex - 1], argv[argIndex]);
        }
        pathv[argIndex - 1] = NULL;
    }
}

void cdCmd(size_t argc, char **argv) {
    if (argc == 2) {
        if (chdir(argv[1]) != 0) {
            handleError();
        }
    // signal an error if there are 0 or >1 arguments
    } else if (argc == 1 || argc > 2) {
        handleError();
    }
}
