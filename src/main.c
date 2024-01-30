#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constants.h"
#include "builtin_commands.h"
#include "exec_command.h"

void runShell();
void parseCmd(int *argc, char cmdInput[MAX_LINE], char argv[MAX_ARGS][MAX_LINE]);

int main() {
    runShell();
    return 0;
}

void runShell() {
    char cmdInput[MAX_LINE];
    char argv[MAX_ARGS][MAX_LINE];
    int argc;

    int pathc = 0;
    char pathv[MAX_PATHS][MAX_LINE];
    while (1) {
        printf("rush> ");
        fflush(stdout);

        // read input and then parse it to get arguments
        fgets(cmdInput, sizeof cmdInput, stdin);
        parseCmd(&argc, cmdInput, argv);

        // execute the appropriate command
        execCommand(&pathc, pathv, argc, argv);
    }
}

void parseCmd(int *argc, char cmdInput[MAX_LINE], char argv[MAX_ARGS][MAX_LINE]) {
    // replace trailing newline with null
    cmdInput[strcspn(cmdInput, "\n")] = '\0';
    char delimiter[] = " \t";
    char *tokenPtr;
    char *savePtr;
    int i = 0;

    tokenPtr = strtok_r(cmdInput, delimiter, &savePtr);
    while (tokenPtr != NULL) {
        strcpy(argv[i], tokenPtr);
        tokenPtr = strtok_r(NULL, delimiter, &savePtr);
        i++;
    }
    strcpy(argv[i], "\0");
    *argc = i;
}