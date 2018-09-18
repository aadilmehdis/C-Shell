#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define TOKEN_BUFFER_SIZE_1 128
#define OUT_DELIMITERS ">"
#define IN_DELIMITERS "<"

int changeInputSource(char *filename);
int changeOutputSource(char *filename);
int changeOutputSourceAppend(char *filename);
void restoreInput(int saved_stdin);
void restoreOuput(int saved_stdout);
char** redirectInputOutput(char **args);
