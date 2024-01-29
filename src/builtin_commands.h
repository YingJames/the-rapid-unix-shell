#ifndef __BUILTIN_COMMANDS_H__
#define __BUILTIN_COMMANDS_H__
// #include <stdio.h>

/**
 * @brief Executes the "path" command.
 *
 * This function is responsible for handling the "path" built-in command in the shell.
 * It takes the number of arguments and an array of argument strings as parameters.
 * The "path" command is used to display or modify the search path for executable files.
 * If the number of arguments is 0, then only built-in commands can be executed
 * 
 * @param argc The number of arguments or number of paths.
 * @param argv An array of argument strings or path values.
 */
char** path(int argc, char **argv);

#endif