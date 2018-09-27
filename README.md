# Linux C-Shell

## Introduction

This is a Linux based POSIX Shell implemented in C.

The Shell has been tested on **Linux** based OS.

## Structure and Features

The application implements a basic Shell which can perform basic Shell operations.

The shell can execute piped commmands.

The shell facilitates the use of redirection.

The shell can perform piping with redirection together.

###The Built-in commands of the shell include

- `ls [-l -a] [file]`

  - The ls command to list all the files and directories in the specified directory.
  - Similarities with conventional ls and prints the contents in the alphabetically sorted order.

- `cd [file]`

  - The cd command changes directory to the directory specified in the given path.

- `echo [arguments]`

  - Displays the arguments in the stdout.

- `exit`

  - Exit the shell.

- `quit`

  - Exit the shell.

- `pwd`

  - Show present working directory.

- `pinfo [pid]`

  - Display the information related to a process.

- `clock`

  - Display the realtime on the terminal.

- `remindme <seconds> <text>`

  - Command to remind the text to user after specified time.

- `fg <Job Number>`

  - Bring a Background or a Stopped process to the Foreground.

- `bg <Job Number>`

  - Run a Stopped process in the Background.

- `jobs`

  - Prints a list of all currently running jobs along with their Process Id.

- `kjob <Job Number> <Signal Number>`

  - Take the job id of a running job and send a signal value to that Process.

- `overkill`

  - Kill all Background process at once.

- `setenv <variable> [value]`

  - Create an environment variable if it does not exist. Change the value of the environment variable set.

- `unsetenv <variable>`

  - Destroy the environment variable.

For all the other commands the shell will fork the current process and create child process and execute the command.

The shell can also run commands in the background. To do so, type `&` argument at the end of the command. (_This feature does not work with inbuilt commands_)

## Running the Shell

1. Run the Makefile
   - `make all`
2. Run the executable

   - `./shell`
