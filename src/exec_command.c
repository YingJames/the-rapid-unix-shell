#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "include/utils.h"
#include "include/builtin_commands.h"
#include "include/exec_command.h"

void execCommand(char **pathv, char **argv) {
    // size_t argc = 0;
    // for (size_t i = 0; i < MAX_ARGS; i++) {
    //     if (argv[i] == NULL) {
    //         argc = i;
    //         break;
    //     }
    // }
    // if (strcmp(argv[0], "path") == 0) {
    //     pathCmd(pathv, argc, argv);
    //     exit(0);
    // }
    // else if (strcmp(argv[0], "cd") == 0) {
    //     cdCmd(argc, argv);
    //     exit(0);
    // }

    // commands outside of builtins
    // else 
        handlePathCommand(pathv, argv); 
}

void handlePathCommand(char **pathv, char **argv) {
    char *fullPath = calloc(MAX_LINE, sizeof(char));
    char *cmd = argv[0]; 

    for (size_t i = 0; pathv[i] != NULL; i++) {
        strcpy(fullPath, pathv[i]);
        strcat(fullPath, "/");
        strcat(fullPath, cmd);
        if (access(fullPath, X_OK) == 0) {
            if (execv(fullPath, argv) == -1) {
                handleError();
                exit(1);
            }
        }
    }
    // if the command is not found in any of the paths
    handleError();
    free(fullPath);
    exit(1);

}

/*
void parseCommand(size_t *argc, char argv[MAX_ARGS][MAX_LINE]) {
    char *outputFile = NULL;
    for (size_t i = 0; i < *argc; i++) {
        if (strcmp(argv[i], ">") == 0) {
            if (i + 1 < *argc) {
                outputFile = argv[i + 1];
                *argc = i;  // Ignore the rest of the command line
                break;
            } else {
                handleError();
                return;
            }
        }
    }
}
*/