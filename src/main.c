#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "include/utils.h"
#include "include/clean_string.h"


int runShell();
int handleCmds(char **cmdLines, char **pathv);


int main(int argc, char **argv) {
    if (argc > 1) {
        handleError();
        exit(1);
    } else {
        runShell();
        exit(0);
    }

    return 0;
}

int strArrLen(char **arr) {
    int i = 0;
    while (arr[i] != NULL) i++;
    return i;
}

int handleCmds(char **cmdLines, char **pathv) {
    char **cmdLine = calloc(3, sizeof(char *));
    cmdLine[0] = calloc(MAX_LINE, sizeof(char));
    cmdLine[1] = calloc(MAX_LINE, sizeof(char));
    cmdLine[2] = NULL;

    // split by args
    char **args = calloc(MAX_ARGS, sizeof(char *));
    for (int j = 0; j < MAX_ARGS; j++) {
        args[j] = calloc(MAX_LINE, sizeof(char));
    }
    for (int i = 0; strcmp(cmdLines[i], "\0") != 0; i++) {
        // split by redirection if any
        int validRedirect = splitString(cmdLine, cmdLines[i], ">");
        if (validRedirect == -1) return -1;
        char *cmd = cmdLine[0];
        char *outputFile = cmdLine[1];

        int validArgs = splitString(args, cmd, " \t");
        if (validArgs == -1) continue;
    }

    // free memory
    for (int i = 0; i < 3; i++)
        free(cmdLine[i]);
    for (int i = 0; i < MAX_ARGS; i++)
        free(args[i]);
    free(cmdLine);
    free(args);
    return 0;
}


int runShell() {
    char *lineInput = NULL;
    size_t lineInputSize = 0;
    char cmdDelimiter[] = "&";
    char argsDelimiter[] = " \t";

    // allocate memory for command strings
    char **cmdLines = calloc(MAX_COMMANDS, sizeof(char *));
    for (int i = 0; i < MAX_COMMANDS; i++) {
        cmdLines[i] = calloc(MAX_LINE, sizeof(char));
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
        fflush(stdin);

        // remove newline character and leading/trailing whitespace
        lineInput[strcspn(lineInput, "\n")] = '\0';
        trim(lineInput);

        // check for exit command
        if (strcmp(lineInput, "exit") == 0) break;
        // check for empty input
        else if (strlen(lineInput) == 0) continue;

        // split input into cmdLines for parallel commands
        int validMultiCmd = splitString(cmdLines, lineInput, cmdDelimiter);
        if (validMultiCmd == -1) continue;

//        int cmdLinesLen = strArrLen(cmdLines);
        int validCmd = handleCmds(cmdLines, pathv);
        if (validCmd == -1) continue;
    }

    // free memory
    for (int i = 0; i < MAX_COMMANDS; i++)
        free(cmdLines[i]);
    for (int i = 0; i < MAX_PATHS; i++)
        free(pathv[i]);
    free(cmdLines);
    free(pathv);
    return 0;
}

//void handleRedirect(char **cmdLines, char **pathv) {
//    // allocate mem for cmd and output filename
//    char **cmd = calloc(2, sizeof(char*));
//    int validRedirect = 0;
//    for (int i = 0; cmdLines[i] != NULL; i++) {
//        // split by redirection
//        validRedirect = splitString(cmd, cmdLines[i], ">");
//        if (validRedirect) break;
//    }
//}