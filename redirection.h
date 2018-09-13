#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void changeInputSource(char *filename);
void changeOutputSource(char *filename);
void changeOutputSourceAppend(char *filename);
void restoreInput(int saved_stdin);
void restoreOuput(int saved_stdout);
char** redirectInputOutput(char **args);
