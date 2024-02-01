#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constants.h"
#include "builtin_commands.h"
#include "exec_command.h"

void runShell();
void cleanupCmd(size_t *argc, char cmdInput[MAX_LINE], char argv[MAX_ARGS][MAX_LINE]);
void rtrim(char *str);

int main() {
    runShell();
    return 0;
}

void runShell() {
    char *string = NULL;
    char cmdInput[MAX_LINE];
    char argv[MAX_ARGS][MAX_LINE];
    size_t argc = 0;
    // ssize_t read;

    size_t pathc = 1;
    char pathv[MAX_PATHS][MAX_LINE];
    strcpy(pathv[0], "/bin");

    while (1) {
        printf("rush> ");
        fflush(stdout);

        getline(&string, &argc, stdin);
        strcpy(cmdInput, string);
        cleanupCmd(&argc, cmdInput, argv);

        // execute the appropriate command
        execCommand(&pathc, pathv, argc, argv);

        // debug path
        // for (size_t i = 0; i < pathc; i++) 
        //     printf("%s\n", pathv[i]);
    }
}

void cleanupCmd(size_t *argc, char cmdInput[MAX_LINE], char argv[MAX_ARGS][MAX_LINE]) {
    // replace trailing newline with null
    cmdInput[strcspn(cmdInput, "\n")] = '\0';
    rtrim(cmdInput);
    char delimiter[] = " \t";
    char *output = NULL;

    size_t i = 0;
    while ((output = strsep(&cmdInput, delimiter)) != NULL) {  
        strcpy(argv[i], output);
        i++;
    }  
    *argc = i;
}

void rtrim(char *str) {
    int len = strlen(str);
    while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\t')) {
        len--;
    }
    str[len] = '\0';
}