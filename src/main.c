#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/utils.h"


void runShell();


int main(int argc, char **argv) {
    if (argc > 1) {
        handleError();
        exit(1);
    } else {
        runShell();
    }

    return 0;
}

void runShell() {
    char *lineInput = NULL;
    size_t lineInputSize = 0;
    char cmdDelimiter = '&';
    char argsDelimiter[] = " \t";

    // allocate memory for command strings
    char **cmdStrings = calloc(MAX_COMMANDS, sizeof(char *));
    for (int i = 0; i < MAX_COMMANDS; i++) {
        cmdStrings[i] = calloc(MAX_LINE, sizeof(char));
    }

    // allocate memory for pathv and default path
    char **pathv = calloc(MAX_PATHS, sizeof(char *));
    for (int i = 0; i < MAX_PATHS; i++) {
        pathv[i] = calloc(MAX_LINE, sizeof(char));
    }
    strcpy(pathv[0], "/bin");

    while (1) {
        printf("rush> ");
        fflush(stdout);

        getline(&lineInput, &lineInputSize, stdin);
    }
}