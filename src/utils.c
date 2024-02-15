#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stddef.h>
#include "include/utils.h"

const size_t MAX_LINE = 255 + 1; /* The maximum length command */
const size_t MAX_ARGS = 64; /* The maximum number of arguments */
const size_t MAX_PATHS = 64; /* The maximum number of paths */
const size_t MAX_COMMANDS = 20;

void handleError(void) {
    char error_message[30] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message));
    fflush(stdout);
}

size_t getStrFreq(char *input, char *targetStr) {
    int count = 0;
    char *tmp = input;

    // count the number of occurrences of targetStr in input
    while ((tmp = strstr(tmp, targetStr)) != NULL) {
        count++;
        tmp++;
    }
    return count;
}

void ltrim(char *str) {
    size_t len = strlen(str);
    size_t i = 0;

    // remove leading whitespace
    while (str[i] == ' ' || str[i] == '\t') {
        i++;
    }

    // shift the string to the left
    if (i > 0) {
        size_t j = 0;
        while (i < len) {
            str[j++] = str[i++];
        }
        str[j] = '\0';
    }
}

void rtrim(char *str) {
    size_t len = strlen(str);

    // remove trailing whitespace
    while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\t')) {
        len--;
    }
    str[len] = '\0';
}

void trim(char *str) {
    ltrim(str);
    rtrim(str);
}

int splitParallelCmds(char *userInput, char **arrayOfStr) {
    char *token, *tempInput, *str;
    tempInput = str = strdup(userInput);

    // split the string with parallel command delimiter
    size_t i = 0;
    while ((token = strsep(&str, "&")) != NULL) {
        trim(token);
        if (strlen(token) == 0) {
            return -1;
        }
        arrayOfStr[i] = realloc(arrayOfStr[i], (strlen(token) + 1) * sizeof(char));

        strcpy(arrayOfStr[i], token);
        strcat(arrayOfStr[i], "\0");
        trim(arrayOfStr[i]);
        i++;
    }
    arrayOfStr[i] = NULL;
    free(tempInput);
    return 0;
}

// skip empty tokens
int splitCmdArgs(char *userInput, char **arrayOfStr) {
    char *token, *tempInput, *str;
    tempInput = str = strdup(userInput);

    size_t i = 0;
    while ((token = strsep(&str, " \t")) != NULL) {
        trim(token);

        if (strlen(token) == 0) {
            continue;
        }
        arrayOfStr[i] = realloc(arrayOfStr[i], (strlen(token) + 1) * sizeof(char));

        strcpy(arrayOfStr[i], token);
        strcat(arrayOfStr[i], "\0");
        trim(arrayOfStr[i]);
        i++;
    }
    arrayOfStr[i] = NULL;
    free(tempInput);
    return 0;
}

// invalid if there are empty tokens
int splitRedirection(char *input, char *outputFile) {
    char *token, *tempInput, *str;
    tempInput = str = strdup(input);

    size_t i = 0;
    while ((token = strsep(&str, ">")) != NULL) {
        trim(token);

        if (strlen(token) == 0) {
            return -1;
        }

        if (i == 0) {
            strcpy(input, token);
            strcat(input, "\0");
        } else if (i == 1) {
            strcpy(outputFile, token);
            strcat(outputFile, "\0");
        }
        i++;
    }

    free(tempInput);
    return 0;
}

void cleanUserInput(char *userInput) {
    // replace trailing newline with null
    userInput[strcspn(userInput, "\n")] = '\0';
    trim(userInput);
}
