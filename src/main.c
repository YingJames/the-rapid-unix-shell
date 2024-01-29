#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 256 /* The maximum length command */

void runShell();
char **parseCmd(char cmdInput[MAX_LINE]);

int main() {
    runShell();
    return 0;
}

void runShell() {
    char cmdInput[MAX_LINE];
    char **args;
    while (strcmp(cmdInput, "exit") != 0) {
        printf("rush> ");
        fgets(cmdInput, sizeof cmdInput, stdin);
        args = parseCmd(cmdInput);
    }
}

char **parseCmd(char cmdInput[MAX_LINE]) {
    // replace trailing newline with null
    cmdInput[strcspn(cmdInput, "\n")] = '\0';
    char delimiter[] = " \t";
    char *tokenPtr;
    char *savePtr;
    char *result[MAX_LINE];
    int i = 0;

    tokenPtr = strtok_r(cmdInput, delimiter, &savePtr);
    while (tokenPtr != NULL) {
        result[i] = tokenPtr;
        tokenPtr = strtok_r(NULL, delimiter, &savePtr);
        i++;
    }
    result[i] = NULL;

    return result;
}

