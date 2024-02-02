#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stddef.h>
#include "utils.h"

const size_t MAX_LINE = 256; /* The maximum length command */
const size_t MAX_ARGS = 64; /* The maximum number of arguments */
const size_t MAX_PATHS = 64; /* The maximum number of paths */

void handleError() {
    char error_message[30] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message)); 
    fflush(stdout);
}

int getStrFreq(size_t argc, char argv[MAX_ARGS][MAX_LINE], char* str) {
    int count = 0;
    for (size_t i = 0; i < argc; i++) {
        if (strcmp(argv[i], str) == 0) {
            count++;
        }
    }
    return count;
}