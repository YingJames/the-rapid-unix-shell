#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>

#include "utils.h"
// #include "builtin_commands.h"
// #include "exec_command.h"

void runShell();
void cleanUserInput(char *userInput);
void splitWithDelimiter(char *userInput, char **arrayOfStr, char delimiter[]);
void rtrim(char *str);

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
    size_t *argcList = calloc(MAX_COMMANDS, sizeof(size_t));

    // allocate memory for command strings
    char **cmdStrings = malloc(MAX_COMMANDS * sizeof(char*));
    for (size_t i = 0; i < MAX_COMMANDS; i++) {
        cmdStrings[i] = calloc(MAX_LINE, sizeof(char));
    }

    char **cmdArgs = malloc(MAX_ARGS * sizeof(char*));
    for (size_t i = 0; i < MAX_ARGS; i++) {
        cmdArgs[i] = calloc(MAX_LINE, sizeof(char));
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

        // debug print cmdStrings
        for (size_t i = 0; cmdStrings[i] != NULL; i++) {
            splitWithDelimiter(cmdStrings[i], cmdArgs, argsDelimiter);
            printf("cmdStrings[%zu]: ", i);
            for (size_t i = 0; cmdStrings[i] != NULL; i++) {
                printf("%s ", cmdArgs[i]);
            }
            printf("\n");
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
    rtrim(userInput);
}

// arrayOfStr is modified
void splitWithDelimiter(char *userInput, char **arrayOfStr, char delimiter[]) {
    char *token, *tempInput, *str;
    tempInput = str = strdup(userInput);

    size_t i = 0;
    while ((token = strsep(&str, delimiter)) != NULL) {
        arrayOfStr[i] = realloc(arrayOfStr[i], (strlen(token) + 1) * sizeof(char));
        strcpy(arrayOfStr[i], token);
        i++;
    }
    arrayOfStr[i] = NULL;
    free(tempInput);
}

void rtrim(char *str) {
    int len = strlen(str);
    while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\t')) {
        len--;
    }
    str[len] = '\0';
}