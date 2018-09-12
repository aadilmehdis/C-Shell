#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int check_hidden(char * file_name);
void set_flags(int *flags, char *arg);
void ls(char *dirname, int hidden);
void getstat(char *filename, char * dirname);
void lls(char *dirname, int hidden);
int b_ls(char **args, char *home_directory);