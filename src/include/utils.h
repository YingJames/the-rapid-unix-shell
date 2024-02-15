#ifndef CONSTANTS_H
#define CONSTANTS_H

extern const size_t MAX_LINE; /* The maximum length command */
extern const size_t MAX_ARGS; /* The maximum number of arguments */
extern const size_t MAX_PATHS; /* The maximum number of paths */
extern const size_t MAX_COMMANDS; /* The maximum number of commands */

// print error message and flush stdout
void handleError(void);

// get the frequency of a string in another string
size_t getStrFreq(char *input, char *targetStr);

// remove leading whitespace
void ltrim(char *str);

// remove trailing whitespace
void rtrim(char *str);

// remove leading and trailing whitespace
void trim(char *str);

// trim and replace newline with null terminator
void cleanUserInput(char *userInput);

// split string with delimiter
int splitParallelCmds(char *userInput, char **arrayOfStr);

// split string for command arguments
int splitCmdArgs(char *userInput, char **arrayOfStr);

// split string for redirection
int splitRedirection(char *input, char *outputFile);

#endif
