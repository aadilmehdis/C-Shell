#include "pwd.h"

int b_pwd(char **args, char *home_directory)  {
    char working_directory[1024];
    getcwd(working_directory,1024);
    printf("%s\n",working_directory);   
    return 1;
}