#include "remindme.h"

/*
 * Function : to_integer
 * ---------------------
 * Convert character string into 
 * an integer.
 */

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
    sleep(atoi(args[1]));
    while(args[i] != NULL)
    {
        printf("%s ",args[i]);
        ++i;
    }
    printf("\n");
    exit(0);
}