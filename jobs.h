#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

int b_jobs(char **args, char *home_directory);
int b_fg(char **args, char *home_directory);
int b_bg(char **args, char *home_directory);
int b_overkill(char **args, char *home_directory);
int b_kjobs(char **args, char *home_directory);
