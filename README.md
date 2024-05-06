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

### Redirection

The shell supports redirection of standard output to a file using the `>` character. This allows users to send the output of a program to a file instead of the screen. 
This command will redirect the output of `ls -la /tmp` to a file named `output`. If the file already exists, it will be overwritten. If the file cannot be opened, an error will be reported.

**Note:** Redirection is supported for external commands only. Redirection for built-in commands (e.g., `path /bin > file`) is not implemented.

### Parallel Commands

The shell allows users to execute commands in parallel using the `&` operator. This feature enables multiple commands to run simultaneously without waiting for each to finish before starting the next.

**Usage:**
```bash
cmd1 & cmd2 [args1 args2] & cmd3 [args1]
```

In this example, `cmd1`, `cmd2`, and `cmd3` will be executed in parallel. The shell will start all specified processes and then use `wait()` or `waitpid` to wait for all of them to complete before returning control to the user.

**Note:** After initiating all processes, the shell ensures that all are completed before accepting new user commands.

### Combining Redirection and Parallel Commands:

You can combine these features to redirect the output of parallel commands into separate files. This is particularly useful for running multiple commands at once and saving their outputs for later review.

**Usage:**
```bash
command1 > output1 & command2 > output2 & command3 > output3
```

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
