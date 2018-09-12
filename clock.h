#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

int to_integer_c(char str[]);
int b_clock(char **args, char *home_directory);
void inthand(int signum);