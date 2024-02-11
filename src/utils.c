#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stddef.h>
#include "include/utils.h"

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