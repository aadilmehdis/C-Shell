#include "remindme.h"

/*
 * Function : to_integer
 * ---------------------
 * Convert character string into 
 * an integer.
 */
int to_integer(char str[])
{
    int len = strlen(str);
    int i, num = 0;
 
    for (i = 0; i < len; i++)
    {
        num = num + ((str[len - (i + 1)] - '0') * pow(10, i));
    }
 
   return num;
}

/*
 * Function : b_remindme
 * ---------------------
 * Takes the command arguments and home directory
 * and reminds the user after a specified number 
 * of seconds the text he types in the command.
 */
int b_remindme(char **args, char *home_directory)
{
    int i=2;
    sleep(to_integer(args[1]));
    while(args[i] != NULL)
    {
        printf("%s ",args[i]);
        ++i;
    }
    printf("\n");
    return 1;
}