#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <ctype.h>

#include "utils.h"
#include "clean_string.h"
#include "builtin_commands.h"
#include "exec_command.h"


int runShell();
int handleCmds(char **cmdLines, char **pathv);


int main(int argc) {
    if (argc > 1) {
        handleError();
        exit(1);
    } else {
        runShell();
        exit(0);
    }

    return 0;
}

int hasWhiteSpace(char *str) {
    for (size_t i = 0; i < strlen(str); i++) {
        if (isspace(str[i])) {
            handleError();
            return -1;
        }
    }
    return 0;
}

void clearStrArray(char **cmdLines) {
    for (int i = 0; i < strArrLen(cmdLines); i++) {
        memset(cmdLines[i], 0, strlen(cmdLines[i]));
    }
}

int handleCmds(char **cmdLines, char **pathv) {
    pid_t pid, wpid;
    int status = 0;
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
    int isValidCmd = 0;
    for (int i = 0; cmdLines[i] != NULL; i++) {
//        clearStrArray(cmdLines);
//        clearStrArray(cmdLine);
//        clearStrArray(argv);
        // split for redirection if necessary
        isValidCmd = splitString(cmdLine, cmdLines[i], ">");
        if (isValidCmd == -1) break;
        char *cmd = cmdLine[0];
        char *outputFile = cmdLine[1];


        // check for multiple redirections in one command
        if (getStrFreq(cmdLines[i], ">") > 1) {
            handleError();
            isValidCmd = -1;
            break;
        }


        // check for valid output file name
        if (outputFile != NULL)
            isValidCmd = hasWhiteSpace(outputFile);
        if (isValidCmd == -1) break;


        // split to get argv
        isValidCmd = splitString(argv, cmd, " \t");
        if (isValidCmd == -1) break;


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
            pid = fork();
            // redirection
            if (pid == 0) {
                if (outputFile != NULL) {
                    close(STDOUT_FILENO);
                    open(outputFile, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
                }
                handlePathCommand(pathv, argv, pid);
            }
        }
    }
    // wait for all children to finish
    while ((wpid = wait(&status)) > 0);

    // free memory
    for (int i = 0; i < 3; i++)
        free(cmdLine[i]);
    for (int i = 0; i < MAX_ARGS; i++)
        free(argv[i]);
    free(cmdLine);
    free(argv);
    if (isValidCmd == -1) return -1;

    return 0;
}


int runShell() {
    char *lineInput = NULL;
    size_t lineInputSize = 0;
    char cmdDelimiter[] = "&";

    // allocate memory for pathv and default path
    char **pathv = calloc(MAX_PATHS, sizeof(char *));
    for (int i = 0; i < MAX_PATHS; i++) {
        pathv[i] = calloc(MAX_LINE, sizeof(char));
    }
    strcpy(pathv[0], "/bin");
    pathv[1] = NULL;

    // allocate memory for command strings
    char **cmdLines = calloc(MAX_COMMANDS, sizeof(char *));
    for (int i = 0; i < MAX_COMMANDS; i++) {
        cmdLines[i] = calloc(MAX_LINE, sizeof(char));
    }

    while (1) {
//        // allocate memory for command strings
//        char **cmdLines = calloc(MAX_COMMANDS, sizeof(char *));
//        for (int i = 0; i < MAX_COMMANDS; i++) {
//            cmdLines[i] = calloc(MAX_LINE, sizeof(char));
//        }
        int isValidCmd = 0;

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
        isValidCmd = splitString(cmdLines, lineInput, cmdDelimiter);
        if (isValidCmd == -1) {
            continue;
        }

        isValidCmd = handleCmds(cmdLines, pathv);
        if (isValidCmd == -1) {
            continue;
        }
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