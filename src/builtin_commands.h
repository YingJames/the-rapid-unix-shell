#ifndef __BUILTIN_COMMANDS_H__
#define __BUILTIN_COMMANDS_H__

/**
 * @brief Sets the path for the shell.
 *
 * This function is responsible for setting the path for the shell.
 * It modifies the path count and the path array using the number of arguments
 * and the argument array as parameters.
 *
 * @param pathc Pointer to the path count variable. This will be modified to reflect the new path count.
 * @param pathv Array of paths. This will be modified to reflect the new paths.
 * @param argc Number of arguments.
 * @param argv Array of arguments.
 */
void pathCmd(int *pathc, char pathv[MAX_PATHS][MAX_LINE], int argc, char argv[MAX_ARGS][MAX_LINE]);

/**
 * @brief Exits the shell.
 *
 * This function is responsible for exiting the shell.
 * It exits the shell if there is only one argument.
 * Otherwise, it prints an error message.
 *
 * @param argc Number of arguments.
 */
void exitCmd(int argc);

#endif