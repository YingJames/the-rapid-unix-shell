#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 256 /* The maximum length command */

void runShell();

int main() {
    runShell();
    return 0;
}

void runShell() {
    char cmdInput[MAX_LINE];
    while (strcmp(cmdInput, "exit") != 0) {
        printf("rush> ");
        fgets(cmdInput, MAX_LINE, stdin);

        // Remove trailing newline, if there is one
        cmdInput[strcspn(cmdInput, "\n")] = '\0';
    }
}

