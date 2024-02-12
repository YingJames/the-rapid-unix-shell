#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "include/utils.h"
#include "include/clean_string.h"
#include "include/builtin_commands.h"
#include "include/exec_command.h"


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


int handleCmds(char **cmdLines, char **pathv) {
    char **cmdLine = calloc(3, sizeof(char *));
    cmdLine[0] = calloc(MAX_LINE, sizeof(char));
    cmdLine[1] = calloc(MAX_LINE, sizeof(char));
    cmdLine[2] = NULL;

    // split by argv
    char **argv = calloc(MAX_ARGS, sizeof(char *));
    for (int j = 0; j < MAX_ARGS; j++) {
        argv[j] = calloc(MAX_LINE, sizeof(char));
    }

    // iterate through all commands
    for (int i = 0; cmdLines[i] != NULL; i++) {
        // split by redirection if any
        int validRedirect = splitString(cmdLine, cmdLines[i], ">");
        if (validRedirect == -1) return -1;
        char *cmd = cmdLine[0];
        char *outputFile = cmdLine[1];

        int validArgs = splitString(argv, cmd, " \t");
        if (validArgs == -1) continue;

        int argc = strArrLen(argv);
        // check for built-in commands
        if (strcmp(argv[0], "path") == 0) {
            pathCmd(pathv, argc, argv);
            continue;
        } else if (strcmp(argv[0], "cd") == 0) {
            cdCmd(argc, argv);
            continue;
        } else if (strcmp(argv[0], "exit") == 0) {
            exitCmd(argc);
            continue;
        } else {
            handlePathCommand(pathv, argv);
        }
    }

    // free memory
    for (int i = 0; i < 3; i++)
        free(cmdLine[i]);
    for (int i = 0; i < MAX_ARGS; i++)
        free(argv[i]);
    free(cmdLine);
    free(argv);
    return 0;
}


int runShell() {
    char *lineInput = NULL;
    size_t lineInputSize = 0;
    char cmdDelimiter[] = "&";
    char argsDelimiter[] = " \t";

    // allocate memory for pathv and default path
    char **pathv = calloc(MAX_PATHS, sizeof(char *));
    for (int i = 0; i < MAX_PATHS; i++) {
        pathv[i] = calloc(MAX_LINE, sizeof(char));
    }
    strcpy(pathv[0], "/bin");
    pathv[1] = NULL;

    while (1) {
        // allocate memory for command strings
        char **cmdLines = calloc(MAX_COMMANDS, sizeof(char *));
        for (int i = 0; i < MAX_COMMANDS; i++) {
            cmdLines[i] = calloc(MAX_LINE, sizeof(char));
        }

        printf("rush> ");
        fflush(stdout);

        getline(&lineInput, &lineInputSize, stdin);
        fflush(stdin);

        // remove newline character and leading/trailing whitespace
        lineInput[strcspn(lineInput, "\n")] = '\0';
        trim(lineInput);

        // check for empty input
        if (strlen(lineInput) == 0) continue;

        // split input into cmdLines for parallel commands
        int validMultiCmd = splitString(cmdLines, lineInput, cmdDelimiter);
        if (validMultiCmd == -1) continue;

//        int cmdLinesLen = strArrLen(cmdLines);
        int validCmd = handleCmds(cmdLines, pathv);
        if (validCmd == -1) continue;

        // free memory
        for (int i = 0; i < MAX_COMMANDS; i++)
            free(cmdLines[i]);
        free(cmdLines);
    }

    for (int i = 0; i < MAX_PATHS; i++)
        free(pathv[i]);
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