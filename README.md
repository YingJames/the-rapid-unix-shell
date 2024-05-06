# The Rapid Unix Shell (RUSH)

RUSH is a custom Unix shell implementation designed for a Unix-based system. This project aims to implement basic shell functionalities, including parallel execution, handling built-in commands, and managing command execution paths.

## Features

- Execution of external commands
- Support for parallel commands and redirection
- Support for built-in commands:
  - `exit`: Exits the shell
  - `cd`: Changes the current working directory
  - `path`: Modifies the path variable for command execution
- Command parsing and execution from the system path
- Handling of command execution errors


## Getting Started

### Prerequisites

Ensure you have `gcc` installed on your system to compile the source code.

### Compilation

To compile RUSH, navigate to the `src` directory and run the following command:

```bash
make all
```
This will compile the source files and generate an executable named rush.

### Running the Shell
After compilation, you can start the shell by running:

```bash
./rush
```
### Usage

Once RUSH is running, you can execute commands just like in any other Unix shell. For example:

```bash
ls -l
```
To use the built-in commands, simply type the command name followed by any necessary arguments. For example, to change the directory:

```bash
cd /path/to/directory
```
To exit the shell, type:

```bash
exit
```

### How the Built-in Commands Work
`exit`: When the user types exit, the shell simply calls the exit system call with 0 as a parameter. It is an error to pass any arguments to exit.

`cd`: cd always take one argument (0 or >1 args should be signaled as an error). To change directories, it uses the chdir() system call with the argument supplied by the user; if chdir fails, that is also an error.

`path`: The path command takes 0 or more arguments, with each argument separated by whitespace from the others. By default, the path is set to `/bin`. A typical usage would be like this: `rush> path /bin /usr/bin`, which would add `/bin` and `/usr/bin` to the search path of the shell. If the user sets path to be empty, then the shell should not be able to run any programs (except built-in commands). The path command always overwrites the old path with the newly specified path.
