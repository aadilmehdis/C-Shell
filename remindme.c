#include "remindme.h"

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