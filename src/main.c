#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "utils.h"
#include "builtin_commands.h"
#include "exec_command.h"

void runShell();
void cleanUserInput(char *userInput);
void splitWithDelimiter(char *userInput, char **arrayOfStr, char delimiter[]);
void rtrim(char *str);
char *ltrim(char *str);

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

        for (size_t i = 0; cmdStrings[i] != NULL; i++) {
            char **cmdArgs = malloc(MAX_ARGS * sizeof(char*));
            for (size_t i = 0; i < MAX_ARGS; i++) {
                cmdArgs[i] = calloc(MAX_LINE, sizeof(char));
            }
            splitWithDelimiter(cmdStrings[i], cmdArgs, argsDelimiter);
            if (strcmp(cmdArgs[0], "exit") == 0) {
                exit(0);
            } else if (strcmp(cmdArgs[0], "") == 0) {
                continue;
            }

            pid_t pid = fork();
            if (pid == 0) {
                // child process
                execCommand(pathv, cmdArgs);
            } else {
                // parent process
                int status;
                waitpid(pid, &status, 0);
            }

            for (size_t i = 0; i < MAX_ARGS; i++) {
                free(cmdArgs[i]);
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
        char *ltrimmedToken = ltrim(token);
        arrayOfStr[i] = realloc(arrayOfStr[i], (strlen(ltrimmedToken) + 1) * sizeof(char));

        strcpy(arrayOfStr[i], ltrimmedToken);
        free(ltrimmedToken);
        i++;
    }
    arrayOfStr[i] = NULL;
    free(tempInput);
}

char* ltrim(char *str) {
    size_t len = strlen(str);
    size_t start = 0;

    while (start < len && (str[start] == ' ' || str[start] == '\t')) {
        start++;
    }

    size_t newLen = len - start;

    char* result = (char*)malloc((newLen + 1) * sizeof(char));

    strncpy(result, str + start, newLen);
    result[newLen] = '\0';
    return result;
}

void rtrim(char *str) {
    int len = strlen(str);
    while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\t')) {
        len--;
    }
    str[len] = '\0';
}