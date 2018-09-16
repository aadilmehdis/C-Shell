#include "jobs.h"

typedef struct {
    char proc_name[500];
    pid_t proc_id;
} process;

extern process PROC_ARR[1000];
int BG_PROC_COUNT;

int b_jobs(char **args, char *home_directory)
{
    for(int i=0 ; i<BG_PROC_COUNT ; ++i)
    {
        printf("[%d]    %s  %s[%d]\n", )
    }
}
