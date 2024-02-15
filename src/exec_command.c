#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "include/utils.h"
#include "include/exec_command.h"

void execCommand(char **pathv, char **argv) {
    char *fullPath = calloc(MAX_LINE, sizeof(char));
    char *cmd = argv[0];

    // check if the command is in the current directory
    for (size_t i = 0; pathv[i] != NULL; i++) {
        strcpy(fullPath, pathv[i]);
        strcat(fullPath, "/");
        strcat(fullPath, cmd);

        // if the command is found in the current directory
        if (access(fullPath, X_OK) == 0) {

            // handle the case where the command is found but the execv fails
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
