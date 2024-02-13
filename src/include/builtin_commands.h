#ifndef __UTILS_H__
#define __UTILS_H__

/**
 * @brief Exits the shell.
 *
 * This function is responsible for exiting the shell.
 * It exits the shell if there is only one argument.
 * Otherwise, it prints an error message.
 *
 * @param argc Number of arguments.
 */
void exitCmd(size_t argc);

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
void pathCmd(char **pathv, size_t argc, char **argv);

/**
 * @brief Changes the current working directory.
 * 
 * This function is responsible for changing the current working directory.
 * It changes the current working directory to the directory specified in the argument.
 * If there are 0 or >1 arguments, it will signal an error.
 * 
 * @param argc Number of arguments.
 * @param argv Array of arguments.
*/
void cdCmd(size_t argc, char **argv);

#endif