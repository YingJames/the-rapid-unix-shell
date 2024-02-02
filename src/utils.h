#ifndef CONSTANTS_H
#define CONSTANTS_H

extern const size_t MAX_LINE; /* The maximum length command */
extern const size_t MAX_ARGS; /* The maximum number of arguments */
extern const size_t MAX_PATHS; /* The maximum number of paths */
extern const size_t MAX_COMMANDS; /* The maximum number of commands */

void handleError();
int getStrFreq(size_t argc, char argv[MAX_ARGS][MAX_LINE], char* str);

#endif 