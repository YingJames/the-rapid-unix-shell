#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "include/utils.h"
#include "include/builtin_commands.h"
#include "include/exec_command.h"

void runShell();
void cleanUserInput(char *userInput);
void splitWithDelimiter(char *userInput, char **arrayOfStr, char delimiter[]);
int splitForRedirection(char *input, char *outputFile);

int main(int argc) {
    if (argc > 1) {
        handleError();
        exit(1);
    }
        runShell();
    return 0;
}

void runBuiltinCmds(int argc, char **cmdArgs, char **pathv) {
    if (argc != 1 && strcmp(cmdArgs[0], "exit") == 0) {
        handleError();
    } else if (strcmp(cmdArgs[0], "exit") == 0) {
        exit(0);
    } else if (strcmp(cmdArgs[0], "path") == 0) {
        pathCmd(pathv, argc, cmdArgs);
    } else if (strcmp(cmdArgs[0], "cd") == 0) {
        cdCmd(argc, cmdArgs);
    }
}
void runShell() {
    pid_t wpid;
    int status = 0;
    char *userInput = NULL;
    size_t userInputSize = 0;
    char cmdDelimiter[] = "&";
    char argsDelimiter[] = " \t";

    // allocate memory for command strings
    char **cmdStrings = malloc(MAX_COMMANDS * sizeof(char*));
    for (size_t i = 0; i < MAX_COMMANDS; i++) {
        cmdStrings[i] = calloc(MAX_LINE, sizeof(char));
    }

    // allocate memory for pathv and default path
    char **pathv = calloc(MAX_PATHS, sizeof(char*));
    for (size_t i = 0; i < MAX_PATHS; i++) {
        pathv[i] = calloc(MAX_LINE, sizeof(char));
    }
    pathv[0] = malloc((strlen("/bin") + 1) * sizeof(char));
    strcpy(pathv[0], "/bin");

    while (1) {
        printf("rush> ");
        fflush(stdout);

        int isValid = getline(&userInput, &userInputSize, stdin);
        if (isValid == -1) {
            handleError();
            continue;
        }

        cleanUserInput(userInput);
        if (strlen(userInput) == 0) {
            continue;
        }
        splitWithDelimiter(userInput, cmdStrings, cmdDelimiter);

        size_t cmdCount = 0;
        for (size_t i = 0; cmdStrings[i] != NULL; i++) {
            cmdCount++;
        }
        pid_t pids[cmdCount];
        int i = 0;

        // check for multiple commands
        int isParallelParent = 0;
        if (cmdCount > 1) {
            for (i = 0; i < cmdCount; i++) {
                pids[i] = fork();
                // if child process break loop
                if (pids[i] == 0) break;
            }
            if (pids[i] != 0) {
                isParallelParent = 1;
                while ((wpid = wait(&status)) > 0);
            }
        }
        if (pids[i] == -1){
            handleError();
        }



//        for (size_t i = 0; cmdStrings[i] != NULL; i++) {
            // check for redirection
        if (isParallelParent != 1) {
            size_t redirectCount = 0;
            char *outputFile = calloc(MAX_LINE, sizeof(char));
            redirectCount = getStrFreq(cmdStrings[i], ">");
            int isValidCmd = splitForRedirection(cmdStrings[i], outputFile);
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
                    if (isspace((unsigned char)outputFile[i])) {
                        handleError();
                        break;
                    }
                }
            }

            // allocate mem for cmdArgs
            char **cmdArgs = malloc(MAX_ARGS * sizeof(char*));
            for (size_t i = 0; i < MAX_ARGS; i++) {
                cmdArgs[i] = calloc(MAX_LINE, sizeof(char));
            }

            // get the cmd args array
            splitWithDelimiter(cmdStrings[i], cmdArgs, argsDelimiter);

            // get argc value
            size_t argc = 0;
            for (size_t i = 0; i < MAX_ARGS; i++) {
                if (cmdArgs[i] == NULL) {
                    argc = i;
                    break;
                }
            }
            // if
            int isBuiltin = -1;
            if (pids[i] != 0) {
                if (argc != 1 && strcmp(cmdArgs[0], "exit") == 0) {
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
            if (isBuiltin == -1) {
                if (cmdCount == 1) pids[i] = fork();
                if (pids[i] < 0) {
                    handleError();
                } else if (pids[i] == 0) {
                    // child process
                    if (strlen(outputFile) > 0) {
                        close(STDOUT_FILENO);
                        open(outputFile, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
                    }
                    execCommand(pathv, cmdArgs);
                } else {
                    // parent process
                    while ((wpid = wait(&status)) > 0);
                }
                for (size_t i = 0; i < MAX_ARGS; i++) {
                    free(cmdArgs[i]);
                }
            }
            free(outputFile);
        }
    }

    for (size_t i = 0; i < MAX_COMMANDS; i++) {
        free(cmdStrings[i]);
    }
    for (size_t i = 0; i < MAX_PATHS; i++) {
        free(pathv[i]);
    }
}

void cleanUserInput( char *userInput) {
    // replace trailing newline with null
    userInput[strcspn(userInput, "\n")] = '\0';
    trim(userInput);
}

// arrayOfStr is modified
void splitWithDelimiter(char *userInput, char **arrayOfStr, char delimiter[]) {
    char *token, *tempInput, *str;
    tempInput = str = strdup(userInput);

    size_t i = 0;
    while ((token = strsep(&str, delimiter)) != NULL) {
        trim(token);
        if (strlen(token) == 0) {
            continue;
        }
        arrayOfStr[i] = realloc(arrayOfStr[i], (strlen(token) + 1) * sizeof(char));

        strcpy(arrayOfStr[i], token);
        // free(token);
        i++;
    }
    arrayOfStr[i] = NULL;
    free(tempInput);
}

int splitForRedirection(char *input, char *outputFile) {
    char *token, *tempInput, *str;
    tempInput = str = strdup(input);

    size_t i = 0;
    while ((token = strsep(&str, ">")) != NULL) {
        trim(token);
        if (strlen(token) == 0) {
            return -1;
        }

        if (i == 0) {
            strcpy(input, token);
            strcat(input, "\0");
        } else if (i == 1) {
            strcpy(outputFile, token);
            strcat(outputFile, "\0");
        }
        i++;
    }

    free(tempInput);
    return 0;
}