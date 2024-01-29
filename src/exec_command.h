#ifndef EXEC_COMMAND_H
#define EXEC_COMMAND_H

/**
 * Executes the given commands.
 *
 * @param pathc The number of paths in the pathv array.
 * @param pathv An array of paths to search for executables.
 * @param argc The number of arguments in the argv array.
 * @param argv An array of command-line arguments.
 */
void execCommand(int pathc, char **pathv, int argc, char **argv);

#endif