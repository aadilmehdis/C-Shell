#include "jobs.h"


typedef struct {
    char proc_name[500];
    pid_t proc_id;
    int state; //0 -> stop 1->running 2->sleeping
} process;

extern process PROC_ARR[1000];
int BG_PROC_COUNT;

int b_jobs(char **args, char *home_directory)
{
    for(int i=0 ; i<BG_PROC_COUNT ; ++i)
    {
        char stat[100];
        if(PROC_ARR[i].state == 0)
        {
            strcpy(stat,"Stopped");
        }
        else
        {
            strcpy(stat,"Running");
        }
        printf("[%d]    %s  %s[%d]\n",i+1, PROC_ARR[i].proc_name,stat, PROC_ARR[i].proc_id);
    }
    return 1;
}

int b_fg(char **args, char *home_directory)
{
    int count=0;
    while(args[count]!=NULL) count++;
    if(count != 2)
    {
        perror("Shell"); return 1;
    }
    else 
    {
        int bring_fg_job = atoi(args[1]);
        if (bring_fg_job > BG_PROC_COUNT)
        {
            perror("Shell : Job does not exist");
            return 1;
        }
        kill(PROC_ARR[bring_fg_job-1].proc_id, SIGCONT);
        PROC_ARR[bring_fg_job-1].proc_id = -9999;
        waitpid(-1, NULL, WUNTRACED);
        process transfer[1000];
        int transfer_count = 0;
        for(int i=0 ;i<BG_PROC_COUNT;++i)
        {
            if(PROC_ARR[i].proc_id != -9999)
            {
                transfer[transfer_count] = PROC_ARR[i];
                ++transfer_count;
            }
        }
        BG_PROC_COUNT = transfer_count;
        for(int i=0;i<BG_PROC_COUNT;++i)
        {
            PROC_ARR[i] = transfer[i];
        }
        return 1;
    }
}

int b_bg(char **args, char *home_directory)
{
    int count=0;
    while(args[count]!=NULL) count++;
    if(count != 2)
    {
        perror("Shell"); return 1;
    }
    else 
    {
        int bring_bg_job = atoi(args[1]);
        if (bring_bg_job > BG_PROC_COUNT)
        {
            perror("Shell : Job does not exist");
            return 1;
        }
        if(PROC_ARR[bring_bg_job-1].state==1)
        {
            perror("Shell : Job already running in background");
            return 1;
        }

        kill(PROC_ARR[bring_bg_job-1].proc_id, SIGCONT);
        PROC_ARR[bring_bg_job-1].state = 1;
        return 1;
    }
}

int b_overkill(char **args, char *home_directory)
{
    for(int i=0;i<BG_PROC_COUNT;++i)
    {
        kill(PROC_ARR[i].proc_id,SIGKILL);
        PROC_ARR[i].proc_id = -9999;
    }
    process transfer[1000];
    int transfer_count = 0;
    for(int i=0 ;i<BG_PROC_COUNT;++i)
    {
        if(PROC_ARR[i].proc_id != -9999)
        {
            transfer[transfer_count] = PROC_ARR[i];
            ++transfer_count;
        }
    }
    BG_PROC_COUNT = transfer_count;
    for(int i=0;i<BG_PROC_COUNT;++i)
    {
        PROC_ARR[i] = transfer[i];
    }
    return 1;
}

int b_kjobs(char **args, char *home_directory)
{
    int count = 0;
    while(args[count] != NULL) count++;
    if(count != 3)
    {
        perror("Shell: kjobs [job_number] [signal_number]");
        return 1;
    }
    else
    {
        int job_number = atoi(args[1]);
        job_number -=1;
        int signal_number = atoi(args[2]);
        if(job_number >= BG_PROC_COUNT)
        {
            perror("Job does not exist.");
            return 1;
        }
        if(kill(PROC_ARR[job_number].proc_id,signal_number) == -1)
        {
            perror("Signal failed");
            return 1;
        }
    }
    return 1;
}
