#ifndef CONSTANTS_H
#define CONSTANTS_H

extern const int MAX_LINE; /* The maximum length command */
extern const int MAX_ARGS; /* The maximum number of arguments */
extern const int MAX_PATHS; /* The maximum number of paths */
extern const int MAX_COMMANDS; /* The maximum number of commands */

void handleError();
size_t getStrFreq(char *input, char* targetStr);

#endif 