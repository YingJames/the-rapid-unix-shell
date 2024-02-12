#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stddef.h>
#include <stdlib.h>
#include "include/utils.h"
#include "include/clean_string.h"

const int MAX_LINE = 255 + 1; /* The maximum length command */
const int MAX_ARGS = 64; /* The maximum number of arguments */
const int MAX_PATHS = 64; /* The maximum number of paths */
const int MAX_COMMANDS = 20;

void handleError() {
    char error_message[30] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message)); 
    fflush(stdout);
}

size_t getStrFreq(char *input, char* targetStr) {
    int count = 0;
    char *tmp = input;
    while((tmp = strstr(tmp, targetStr)) != NULL) {
        count++;
        tmp++;
    }
    return count;
}

int splitString(char **arrayOfStr, char *userInput, char delimiter[]) {
    char *token, *tempInput, *str;
    tempInput = str = strdup(userInput);

    size_t i = 0;
    while ((token = strsep(&str, delimiter)) != NULL) {
        trim(token);

        // allocate memory
        size_t len = strlen(token);

        if (len == 0) {
            handleError();
            return -1;
        }
        strcpy(arrayOfStr[i], token);
        arrayOfStr[i][len] = '\0';

        i++;
    }
    arrayOfStr[i] = NULL;
    free(tempInput);
    return 0;
}