#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "utils.h"
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
        execFromPath(pathc, pathv, argc, argv);
    }
}

void execFromPath(size_t *pathc, char pathv[MAX_PATHS][MAX_LINE], int argc, char argv[MAX_ARGS][MAX_LINE]) {
    char fullPath[MAX_ARGS];
    char *cmd = argv[0]; 
    for (size_t i = 0; i < *pathc; i++) {
        strcpy(fullPath, pathv[i]);
        strcat(fullPath, "/");
        strcat(fullPath, cmd);
        if (access(fullPath, X_OK) == 0) {
            char *argvPointers[MAX_ARGS+1];
            for (int i = 0; i < argc; i++)
                argvPointers[i] = argv[i];

            argvPointers[argc] = NULL;

            pid_t pid = fork();
            if (pid == 0) {
                // child process
                execv(fullPath, argvPointers);
            } else {
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