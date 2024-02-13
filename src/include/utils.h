#ifndef CONSTANTS_H
#define CONSTANTS_H

extern const size_t MAX_LINE; /* The maximum length command */
extern const size_t MAX_ARGS; /* The maximum number of arguments */
extern const size_t MAX_PATHS; /* The maximum number of paths */
extern const size_t MAX_COMMANDS; /* The maximum number of commands */

void handleError();
size_t getStrFreq(char *input, char* targetStr);
void ltrim(char *str);
void rtrim(char *str);
void trim(char *str);

#endif 