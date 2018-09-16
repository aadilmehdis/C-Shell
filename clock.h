#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int b_clock(char **args, char *home_directory);
void inthand(int signum);