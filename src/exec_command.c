#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
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

    // commands outside of builtins
    else {
        char fullPath[MAX_LINE];
        char *cmd = argv[0]; 
        for (size_t i = 0; i < *pathc; i++) {
            strcpy(fullPath, pathv[i]);
            strcat(fullPath, "/");
            strcat(fullPath, cmd);
            if (access(fullPath, X_OK) == 0) {
                char *argvPointers[MAX_ARGS+1];
                for (int i = 0; i < argc; i++) {
                    argvPointers[i] = argv[i];
                }
                argvPointers[argc] = NULL;

                pid_t pid = fork();

                // child process
                if (pid == 0)
                    execv(fullPath, argvPointers);
                else {
                    // parent process
                    int status;
                    waitpid(pid, &status, 0);
                }
            } else {
                handleError();
                break;
            }
        }
    }
}