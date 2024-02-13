//
// Created by james on 2/11/24.
//

#include "clean_string.h"
#include <string.h>


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






