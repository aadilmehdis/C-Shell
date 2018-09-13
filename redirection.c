#include "redirection.h"

void changeInputSource(char *filename)
{
    int in;
    // printf("checkpoint 3\n");
    in = open(filename, O_RDONLY);
    // printf("checkpoint 4\n");
    if(in == -1)
    {
        printf("%s\n", filename);
        perror("Shell");
        exit(1);
    }
    dup2(in, 0);
    // printf("checkpoint 5\n");
    close(in);
}

void changeOutputSource(char *filename)
{
    int out;
    out = creat(filename, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
    if(out == -1)
    {
        perror("Shell");
        exit(1);
    }
    dup2(out, 1);
    close(out);
}

void changeOutputSourceAppend(char *filename)
{
    int out;
    out = open(filename, O_WRONLY | O_APPEND);
    if(out == -1)
    {
        perror("Shell");
        exit(1);
    }
    dup2(out, 1);
    close(out);
}

void restoreInput(int saved_stdin)
{
    dup2(saved_stdin, 0);
    close(saved_stdin);
}

void restoreOuput(int saved_stdout)
{
    dup2(saved_stdout, 1);
    close(saved_stdout);
}


char** redirectInputOutput(char **args)
{
    int newargscnt = 0;
    int encountered_redirection = 0;
    char **newargs = malloc(sizeof(char *) * 500);
    for(int i=0 ; args[i]!=NULL ; ++i)
    {
        if(strcmp(args[i], "<")==0)
        {
            if(args[i+1] == NULL)
            {
                perror("Shell");
                exit(1);
            }
            else
            {
                // printf("checkpoint 1\n");
                changeInputSource(args[i+1]);
                // printf("checkpoint 2\n");
            }
            encountered_redirection = 1;
        }
        else if(strcmp(args[i],">")==0)
        {
            if(args[i+1] == NULL)
            {
                perror("Shell");
                exit(1);
            }
            else
            {
                changeOutputSource(args[i+1]);
            }   
            encountered_redirection = 1;
        }
        else if(strcmp(args[i], ">>")==0)
        {
            if(args[i+1] == NULL)
            {
                perror("Shell");
                exit(1);
            }
            else
            {
                changeOutputSourceAppend(args[i+1]);
            }
            encountered_redirection = 1;
        }
        else if(!encountered_redirection)
        {
            newargs[newargscnt] = args[i];
            ++newargscnt;
        }
    }
    return newargs;
}
