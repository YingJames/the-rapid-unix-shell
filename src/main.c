#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "builtin_commands.h"
#include "exec_command.h"

void runShell();
void cleanupUserInput(size_t *argc, char userInput[MAX_LINE], char argv[MAX_ARGS][MAX_LINE]);
void splitCommands(size_t *inputStringsCount, size_t argcList[20], char userInputStrings[MAX_ARGS][MAX_LINE], char allCommands[20][MAX_ARGS][MAX_LINE]);
void rtrim(char *str);

int main() {
    runShell();
    return 0;
}

void runShell() {
    char *string = NULL;
    char userInput[MAX_LINE];
    char allCommands[20][MAX_ARGS][MAX_LINE];
    char userInputStrings[MAX_ARGS][MAX_LINE];
    // char argv[MAX_ARGS][MAX_LINE];
    size_t inputStringsCount = 0;
    // size_t argc = 0;
    size_t argcList[20];
    // ssize_t read;


    size_t pathc = 1;
    char pathv[MAX_PATHS][MAX_LINE];
    strcpy(pathv[0], "/bin");

    while (1) {
        printf("rush> ");
        fflush(stdout);

        getline(&string, &inputStringsCount, stdin);
        strcpy(userInput, string);
        cleanupUserInput(&inputStringsCount, userInput, userInputStrings);
        splitCommands(&inputStringsCount, argcList, userInputStrings, allCommands);
        size_t commandCount = (size_t)getStrFreq(inputStringsCount, userInputStrings, "&") + 1;

        // execute the appropriate command
        for (size_t i = 0; i < commandCount; i++) {
            execCommand(&pathc, pathv, argcList[i], allCommands[i]);
        }
        // execCommand(&pathc, pathv, argc, argv);

        // debug path
        // printf("Your input: \n");
        // for (size_t i = 0; i < inputStringsCount; i++) {
        //     printf("%s ", userInputStrings[i]);
        // }
        // printf("\n");

        // printf("Each individual command: \n");
        // for (size_t commandIndex = 0; commandIndex < commandCount; commandIndex++) {
        //     for (size_t argIndex = 0; argIndex < argcList[commandIndex]; argIndex++) {
        //         printf("%s ", allCommands[commandIndex][argIndex]);
        //     }
        //     printf("\n");
        // }
    }
}

void cleanupUserInput(size_t *inputStringsCount, char userInput[MAX_LINE], char userInputStrings[MAX_ARGS][MAX_LINE]) {
    // replace trailing newline with null
    userInput[strcspn(userInput, "\n")] = '\0';
    rtrim(userInput);
    char delimiter[] = " \t";
    char *output = NULL;

    size_t i = 0;
    while ((output = strsep(&userInput, delimiter)) != NULL) {  
        strcpy(userInputStrings[i], output);
        i++;
    }  
    *inputStringsCount = i;
}

void splitCommands(size_t *inputStringsCount, size_t argcList[20], char userInputStrings[MAX_ARGS][MAX_LINE], char allCommands[20][MAX_ARGS][MAX_LINE]) {
    size_t commandIndex = 0;
    size_t argIndex = 0;
    for (size_t i = 0; i < *inputStringsCount; i++) {
        if (strcmp(userInputStrings[i], "&") == 0) {
            argcList[commandIndex] = argIndex;
            commandIndex++;
            argIndex = 0;
        } else {
            strcpy(allCommands[commandIndex][argIndex], userInputStrings[i]);
            argIndex++;
        }
    }
    // add the last command count
    argcList[commandIndex] = argIndex;
}

void rtrim(char *str) {
    int len = strlen(str);
    while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\t')) {
        len--;
    }
    str[len] = '\0';
}