#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "utils.h"
#include "builtin_commands.h"
#include "exec_command.h"

void runShell();
void cleanUserInput(char *userInput);
void splitWithDelimiter(char *userInput, char **arrayOfStr, char delimiter[]);
void splitForRedirection(char *input, char *outputFile);
void rtrim(char *str);
void ltrim(char *str);

int main(int argc) {
    if (argc > 1) {
        handleError();
        exit(1);
    }
        runShell();
    return 0;
}

void runShell() {
    char *userInput = NULL;
    size_t userInputSize = 0;
    char cmdDelimiter[] = "&";
    char argsDelimiter[] = " \t";

    // allocate memory for argcList
    // size_t *argcList = calloc(MAX_COMMANDS, sizeof(size_t));

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

        getline(&userInput, &userInputSize, stdin);
        cleanUserInput(userInput);
        splitWithDelimiter(userInput, cmdStrings, cmdDelimiter);

        size_t cmdCount = 0;
        for (size_t i = 0; cmdStrings[i] != NULL; i++) {
            cmdCount++;
        }
        // int isValid = 1;
        // if (cmdCount > 1) {
        //     for (size_t i = 0; i < cmdCount; i++) {
        //         if (strlen(cmdStrings[i]) == 0) {
        //             handleError();
        //             isValid = 0;
        //             break;
        //         }
        //     }
        // }
        // if (!isValid) {
        //     continue;
        // }
        pid_t pids[cmdCount];

        for (size_t i = 0; cmdStrings[i] != NULL; i++) {
            // check for redirection
            size_t redirectCount = 0;
            char *outputFile = calloc(MAX_LINE, sizeof(char));
            redirectCount = getStrFreq(cmdStrings[i], ">");
            splitForRedirection(cmdStrings[i], outputFile);
            size_t commandStrLen = strlen(cmdStrings[i]);
            size_t outputFileStrLen = strlen(outputFile);
            if (redirectCount > 1 || (commandStrLen == 0 || outputFileStrLen == 0)) {
                handleError();
                continue;
            } else {
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
            if (argc != 1 && strcmp(cmdArgs[0], "exit") == 0) {
                handleError();
            } else if (strcmp(cmdArgs[0], "exit") == 0) {
                exit(0);
            } else if (strcmp(cmdArgs[0], "") == 0) {
                continue;
            } else if (strcmp(cmdArgs[0], "path") == 0) {
                pathCmd(pathv, argc, cmdArgs);
            } else if (strcmp(cmdArgs[0], "cd") == 0) {
                cdCmd(argc, cmdArgs);
            } else {
                pid_t pid = fork();
                if (pid == 0) {
                    // child process
                    if (strlen(outputFile) > 0) {
                        close(STDOUT_FILENO);
                        open(outputFile, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
                    }
                    execCommand(pathv, cmdArgs);
                } else {
                    // parent process
                    pids[i] = pid;
                }

                for (size_t i = 0; i < MAX_ARGS; i++) {
                    free(cmdArgs[i]);
                }
            }

            int status;
            for (size_t i = 0; i < cmdCount; i++) {
                if (pids[i] != 0) {
                    waitpid(pids[i], &status, 0);
                }
            }
            // debug print pathv
            // printf("pathv: ");
            // for (size_t i = 0; pathv[i] != NULL; i++) {
            //     printf("%s ", pathv[i]);
            // }
            // printf("\n");

            // debug print cmdStrings
            // printf("cmdStrings[%zu]: ", i);
            // for (size_t j = 0; cmdArgs[j] != NULL; j++) {
            //     printf("%s ", cmdArgs[j]);
            // }
            // printf("\n");

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
    ltrim(userInput);
    rtrim(userInput);
}

// arrayOfStr is modified
void splitWithDelimiter(char *userInput, char **arrayOfStr, char delimiter[]) {
    char *token, *tempInput, *str;
    tempInput = str = strdup(userInput);

    size_t i = 0;
    while ((token = strsep(&str, delimiter)) != NULL) {
        ltrim(token);
        rtrim(token);
        arrayOfStr[i] = realloc(arrayOfStr[i], (strlen(token) + 1) * sizeof(char));

        strcpy(arrayOfStr[i], token);
        // free(token);
        i++;
    }
    arrayOfStr[i] = NULL;
    free(tempInput);
}

void splitForRedirection(char *input, char *outputFile) {
    char *token, *tempInput, *str;
    tempInput = str = strdup(input);

    size_t i = 0;
    while ((token = strsep(&str, ">")) != NULL) {
        ltrim(token);
        rtrim(token);

        if (i == 0) {
            strcpy(input, token);
        } else if (i == 1) {
            strcpy(outputFile, token);
        }
        i++;
    }
    free(tempInput);
}

// char* ltrim(char *str) {
//     size_t len = strlen(str);
//     size_t start = 0;

//     while (start < len && (str[start] == ' ' || str[start] == '\t')) {
//         start++;
//     }

//     size_t newLen = len - start;

//     char* result = (char*)malloc((newLen + 1) * sizeof(char));

//     strncpy(result, str + start, newLen);
//     result[newLen] = '\0';
//     return result;
// }
void ltrim(char *str) {

    size_t len = strlen(str);
    size_t i = 0;

    while (str[i] == ' ' || str[i] == '\t') {
        i++;
    }

    if (i > 0) {
        size_t j = 0;
        while (i < len) {
            str[j++] = str[i++];
        }
        str[j] = '\0'; 
    }

}

void rtrim(char *str) {
    int len = strlen(str);
    while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\t')) {
        len--;
    }
    str[len] = '\0';
}