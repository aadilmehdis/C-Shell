#include <sys/types.h> 
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int b_pinfo(char **args, char *home_directory);
int read_ith_line(char * filename, int line_number, char * line);
int check_file_exists(const char * filename);

