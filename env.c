#include "env.h"

int b_setenv(char **args, char *home_directory)
{
    int count = 0;
    while(args[count]!=NULL) count++;
    if(count<2)
    {
        perror("Shell: sentenv [var] [value]");
        return 1;
    }
    if(count==2)
    {
        setenv(args[1], "", 1);
        return 1;
    }
    setenv(args[1], args[2], 1);
    return 1;
}

int b_unsetenv(char **args, char *home_directory)
{
    int count = 0;
    while(args[count]!=NULL) count++;
    if(count!=2)
    {
        perror("Shell: unsentenv [var]");
        return 1;
    }
    unsetenv(args[1]);
    return 1;
}