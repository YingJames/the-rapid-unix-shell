#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stddef.h>
#include "include/utils.h"

const size_t MAX_LINE = 255 + 1; /* The maximum length command */
const size_t MAX_ARGS = 64; /* The maximum number of arguments */
const size_t MAX_PATHS = 64; /* The maximum number of paths */
const size_t MAX_COMMANDS = 20; 

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

void ltrim(char *str) {
    size_t len = strlen(str);
    size_t i = 0;

    while (str[i] == ' ' || str[i] == '\t') {
        i++;
    }

    if (i > 0) {
        size_t j = 0;
        while (i < len) {
            str[j++] = str[i++];
        }
        str[j] = '\0';
    }
}

void rtrim(char *s) {
    size_t len = strlen(s);
    while (len > 0 && (s[len-1] == ' ' || s[len-1] == '\t')) {
        len--;
    }
    s[len] = '\0';
}

void trim(char *s) {
    ltrim(s);
    rtrim(s);
}

