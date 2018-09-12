#include "clock.h"


extern volatile sig_atomic_t STOP;

int to_integer_c(char str[])
{
    int len = strlen(str);
    int i, num = 0;
 
    for (i = 0; i < len; i++)
    {
        num = num + ((str[len - (i + 1)] - '0') * pow(10, i));
    }
 
   return num;
}

void inthand(int signum)
{
    STOP = 1;
}

int b_clock(char **args, char *home_directory)
{
    STOP = 0;
    signal(SIGINT, inthand);
    int interval = 1;
    if(args[1] != NULL)
        interval = to_integer_c(args[1]);

    while(!STOP){
        time_t raw_time;
        struct tm* time_;
        time(&raw_time);
        time_ = localtime(&raw_time);
        char t[100] = "";
        strftime(t, 100, "%d %b %y, %H:%M:%S\n", time_);
        printf("%s", t);
        sleep(interval);
    }
    return 1;
}