#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

#include "include/utils.h"
#include "include/builtin_commands.h"
#include "include/exec_command.h"

void runShell(void);

int main(int argc) {
    if (argc > 1) {
        handleError();
        exit(1);
    }
    runShell();
    return 0;
}

void runShell(void) {
    pid_t wpid;
    int status = 0;
    char *userInput = NULL;
    size_t userInputSize = 0;

    // allocate memory for command strings
    char **cmdStrings = malloc(MAX_COMMANDS * sizeof(char *));
    for (size_t i = 0; i < MAX_COMMANDS; i++) {
        cmdStrings[i] = calloc(MAX_LINE, sizeof(char));
    }

    // allocate memory for pathv and default path
    char **pathv = calloc(MAX_PATHS, sizeof(char *));
    for (size_t i = 0; i < MAX_PATHS; i++) {
        pathv[i] = calloc(MAX_LINE, sizeof(char));
    }
    pathv[0] = malloc((strlen("/bin") + 1) * sizeof(char));
    strcpy(pathv[0], "/bin");

    // main loop
    while (1) {
        printf("rush> ");
        fflush(stdout);

        // handle user input
        int isValid = getline(&userInput, &userInputSize, stdin);
        if (isValid == -1) {
            handleError();
            continue;
        }
        // remove newline and whitespace
        cleanUserInput(userInput);

        // check for empty input
        if (strlen(userInput) == 0) {
            continue;
            // check for too long input
        } else if (userInputSize > MAX_LINE) {
            handleError();
            continue;
        }

        // split parallel commands if any
        isValid = splitParallelCmds(userInput, cmdStrings);
        if (isValid == -1) {
            handleError();
            continue;
        }

        // get the number of commands
        int cmdCount = 0;
        for (int i = 0; cmdStrings[i] != NULL; i++) {
            cmdCount++;
        }

        // fork for parallel commands
        int cmdIndex = 0;
        pid_t pids[cmdCount];
        int isParallelParent = 0;
        if (cmdCount > 1) {
            // if parent process, fork for each command
            for (cmdIndex = 0; cmdIndex < cmdCount; cmdIndex++) {
                pids[cmdIndex] = fork();
                // if child process break loop
                if (pids[cmdIndex] == 0) break;
            }

            // if parent process, wait for all children
            if (pids[cmdIndex] != 0) {
                isParallelParent = 1;
                while ((wpid = wait(&status)) > 0);
            }
        }
        // if fork fails handle error
        if (pids[cmdIndex] == -1) {
            handleError();
        }

        if (isParallelParent != 1) {

            // handle redirection
            size_t redirectCount = 0;
            char *outputFile = calloc(MAX_LINE, sizeof(char));
            redirectCount = getStrFreq(cmdStrings[cmdIndex], ">");
            int isValidCmd = splitRedirection(cmdStrings[cmdIndex], outputFile);

            if (isValidCmd == -1) {
                // check for empty or invalid redirection
                handleError();
                continue;
            } else if (redirectCount > 1) {
                // check for multiple redirection
                handleError();
                continue;
            } else {
                // check for spaces in output file
                for (int i = 0; outputFile[i]; i++) {
                    if (isspace((unsigned char) outputFile[i])) {
                        handleError();
                        break;
                    }
                }
            }

            // allocate memory for command args
            char **cmdArgs = malloc(MAX_ARGS * sizeof(char *));
            for (size_t i = 0; i < MAX_ARGS; i++) {
                cmdArgs[i] = calloc(MAX_LINE, sizeof(char));
            }

            // get the cmd args array
            splitCmdArgs(cmdStrings[cmdIndex], cmdArgs);

            // get args count
            size_t argc = 0;
            for (size_t i = 0; i < MAX_ARGS; i++) {
                if (cmdArgs[i] == NULL) {
                    argc = i;
                    break;
                }
            }

            // check for builtin commands
            int isBuiltin = -1;
            if (pids[cmdIndex] != 0) {
                if (argc != 1 && strcmp(cmdArgs[0], "exit") == 0) {
                    isBuiltin = 1;
                    handleError();
                } else if (strcmp(cmdArgs[0], "exit") == 0) {
                    exit(0);
                } else if (strcmp(cmdArgs[0], "path") == 0) {
                    isBuiltin = 1;
                    pathCmd(pathv, argc, cmdArgs);
                } else if (strcmp(cmdArgs[0], "cd") == 0) {
                    isBuiltin = 1;
                    cdCmd(argc, cmdArgs);
                }
            }

            // if not a builtin command
            if (isBuiltin == -1) {
                // if not parallel, then fork
                if (cmdCount == 1)
                    pids[cmdIndex] = fork();

                // if fork fails
                if (pids[cmdIndex] < 0)
                    handleError();

                // child process
                else if (pids[cmdIndex] == 0) {
                    // redirect output to file
                    if (strlen(outputFile) > 0) {
                        close(STDOUT_FILENO);
                        open(outputFile, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
                    }
                    execCommand(pathv, cmdArgs);
                } else {
                    // parent process wait for all children
                    while ((wpid = wait(&status)) > 0);
                }

                // free memory
                for (size_t i = 0; i < MAX_ARGS; i++) {
                    free(cmdArgs[i]);
                }
            }
            free(outputFile);
        }
    }
}
