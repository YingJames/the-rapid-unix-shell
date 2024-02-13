#ifndef EXEC_COMMAND_H
#define EXEC_COMMAND_H

/**
 * @brief Executes a command
 * 
 * This function is responsible for executing a built-in command or one from the path.
 * It uses the path count and the path array to find where the commands are located.
 * 
 * @param pathc Number of paths in pathv
 * @param pathv Array of paths
 * @param argc Number of arguments
 * @param argv Array of arguments
*/
void execCommand(char **pathv, char **argv);
void handlePathCommand(char **pathv, char **argv);
// void parseCommand(size_t *argc, char argv[MAX_ARGS][MAX_LINE]);

#endif