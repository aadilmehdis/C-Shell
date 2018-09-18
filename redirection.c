#include "redirection.h"

int changeInputSource(char *filename)
{
    int in;
    // printf("checkpoint 3\n");
    in = open(filename, O_RDONLY);
    // printf("checkpoint 4\n");
    if(in == -1)
    {
        // printf("%s\n", filename);
        perror("Shell (Press Ctrl+C to continue)");
        return -1;
    }
    dup2(in, 0);
    // printf("checkpoint 5\n");
    close(in);
    return 1;
}

int changeOutputSource(char *filename)
{
    int out;
    out = creat(filename, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
    if(out == -1)
    {
        perror("Shell (Press Ctrl+C to continue)");
        return -1;
    }
    dup2(out, 1);
    close(out);
    return 1;
}

int changeOutputSourceAppend(char *filename)
{
    int out;
    out = open(filename, O_WRONLY | O_APPEND);
    if(out == -1)
    {
        perror("Shell (Press Ctrl+C to continue)");
        return -1;
    }
    dup2(out, 1);
    close(out);
    return 1;
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


char **redirectInputOutput(char **args)
{
    int newargscnt = 0;
    int encountered_redirection = 0;
    char **newargs = malloc(sizeof(char *) * 500);
    for(int i=0;i<500;++i)
    {
        newargs[i] = NULL;
    }
    for(int i=0 ; args[i]!=NULL ; ++i)
    {
        if(strcmp(args[i], "<")==0)
        {
            if(args[i+1] == NULL)
            {
                errno = 2;
                perror("Shell");
                // return NULL;
                // char **return_error = malloc(sizeof(char *) * 2);
                // strcpy(return_error[0],"-9999");
                // return_error[1] = NULL;
                // return return_error;
            }
            else
            {
                // printf("checkpoint 1\n");
                changeInputSource(args[i+1]);
                // {
                //     perror("Shell");
                //     // return NULL;
                //     // char **return_error = malloc(sizeof(char *) * 2);
                //     // strcpy(return_error[0],"-9999");
                //     // return_error[1] = NULL;
                //     // return return_error;
                // }
                // printf("checkpoint 2\n");
            }
            encountered_redirection = 1;
        }
        else if(strcmp(args[i],">")==0)
        {
            if(args[i+1] == NULL)
            {
                errno = 2;
                perror("Shell");
                // return NULL;
                // char **return_error = malloc(sizeof(char *) * 2);
                // strcpy(return_error[0],"-9999");
                // return_error[1] = NULL;
                // return return_error;
            }
            else
            {
                changeOutputSource(args[i+1]);
                    // perror("Shell");
                    // return NULL;
                    // char **return_error = malloc(sizeof(char *) * 2);
                    // strcpy(return_error[0],"-9999");
                    // return_error[1] = NULL;
                    // return return_error;
                
            }   
            encountered_redirection = 1;
        }
        else if(strcmp(args[i], ">>")==0)
        {
            if(args[i+1] == NULL)
            {
                errno = 2;
                perror("Shell");
                // return NULL;
                // char **return_error = malloc(sizeof(char *) * 2);
                // strcpy(return_error[0],"-9999");
                // return_error[1] = NULL;
                // return return_error;
            }
            else
            {
                changeOutputSourceAppend(args[i+1]);
                // {
                //     perror("Shell");
                //     // return NULL;
                //     // char **return_error = malloc(sizeof(char *) * 2);
                //     // strcpy(return_error[0],"-9999");
                //     // return_error[1] = NULL;
                //     // return return_error;
                // }
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


char **parseInputOutRedirection(char *input_line) {
    int buffer_size = TOKEN_BUFFER_SIZE_1;
    int position = 0;
    char **token_list = malloc(buffer_size * sizeof(char *));
    char *token;

    if(!token_list) {
        fprintf(stderr, "Allocation Error");
        exit(1); // EXIT_FAILURE
    }

    token = strtok(input_line, OUT_DELIMITERS);
    while(token != NULL) {
        token_list[position] = token;
        position++;

        if(position >= buffer_size) {
            buffer_size += TOKEN_BUFFER_SIZE_1;
            token_list = realloc(token_list, buffer_size * sizeof(char *));
            if(!token_list) {
                fprintf(stderr, "Allocation Error");
                exit(1); // EXIT_FAILURE
            }
        }
        token = strtok(NULL, OUT_DELIMITERS);
    }
    token_list[position] = NULL;
    return token_list;
}

char **parseInputInRedirection(char *input_line) {
    int buffer_size = TOKEN_BUFFER_SIZE_1;
    int position = 0;
    char **token_list = malloc(buffer_size * sizeof(char *));
    char *token;

    if(!token_list) {
        fprintf(stderr, "Allocation Error");
        exit(1); // EXIT_FAILURE
    }

    token = strtok(input_line, IN_DELIMITERS);
    while(token != NULL) {
        token_list[position] = token;
        position++;

        if(position >= buffer_size) {
            buffer_size += TOKEN_BUFFER_SIZE_1;
            token_list = realloc(token_list, buffer_size * sizeof(char *));
            if(!token_list) {
                fprintf(stderr, "Allocation Error");
                exit(1); // EXIT_FAILURE
            }
        }
        token = strtok(NULL, IN_DELIMITERS);
    }
    token_list[position] = NULL;
    return token_list;
}