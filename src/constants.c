#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stddef.h>
#include "constants.h"

const size_t MAX_LINE = 256; /* The maximum length command */
const size_t MAX_ARGS = 64; /* The maximum number of arguments */
const size_t MAX_PATHS = 64; /* The maximum number of paths */

void handleError() {
    char error_message[30] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message)); 
    fflush(stdout);
}