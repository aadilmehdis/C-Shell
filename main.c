#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "exit.h"
#include "cd.h"
#include "pwd.h"
#include "echo.h"
#include "remindme.h"
#include "clock.h"
#include "pinfo.h"

#define TOKEN_BUFFER_SIZE 128
#define TOKEN_DELIMITERS " \t\r\n\a"
#define SEMICOLON_DELIMITERS ";"

void shellLoop(void);
int executeCommand(char **args);
int launchProgram(char **args);
char *readInput(void);
char **parseInput(char *input_line);
char **parseInputSemiColon(char *input_line);
void printPrompt(void);
int number_builtin(void);
void checkBackgroundCompleted(void);


int b_ls(char **args, char *home_directory);

typedef struct {
    char proc_name[500];
    pid_t proc_id;
} process;


// GLOBAL VARIABLES
process PROC_ARR[1000];
int BG_PROC_COUNT = 0;
char HOME_DIR[1024];
char CURRENT_DIR[1024];
char *USERNAME;
char HOSTNAME[1024];
pid_t GLOBAL_PID;
volatile sig_atomic_t STOP;


char *str_builtin[] = {
    "cd",
    "pwd",
    "echo",
    "exit",
    "ls",
    "clock",
    "pinfo"
};

char *str_builtin_bg[] = {
    "remindme"
};

int (*func_builtin[])(char **, char *) = {
      &b_cd, 
      &b_pwd, 
      &b_echo, 
      &b_exit, 
      &b_ls,
      &b_clock, 
      &b_pinfo
};

int (*func_builtin_bg[])(char **, char *) = {
    &b_remindme
};


int main(int argc, char **argv)
{
    getcwd(HOME_DIR,1024);
    GLOBAL_PID = getpid();
    shellLoop();
    return 0;
}


void shellLoop(void) {
    char *input_line;
    char **args;
    int return_status = 1;

    do {
        printPrompt();
        checkBackgroundCompleted();
        input_line = readInput();
        args = parseInputSemiColon(input_line);
        int count_commands = 0;
        while(args[count_commands] != NULL) ++count_commands; // count the number of semicolon seperated commmands given in a single line
        for(int i=0 ; i < count_commands && return_status ; ++i)
        {
            char ** innerargs = parseInput(args[i]);
            return_status = executeCommand(innerargs);
        }
        free(input_line);
        free(args);
    } while(return_status);
}

int executeCommand(char **args) {
    if(args[0] == NULL) {
        return 1;
    }

    for (int i = 0 ; i < number_builtin() ; ++i) {
        if(strcmp(args[0], str_builtin[i]) == 0) {
            return (*func_builtin[i])(args, HOME_DIR);
        }
    }

    return launchProgram(args);
}

int checkRunBG(char **args) {
    int last_arg = 0;
    while(args[last_arg]!=NULL) ++last_arg; 
    --last_arg;
    if(strcmp(args[last_arg],"&")==0) {
        return last_arg;
    }
    return -1;
}

int number_builtin() {
    return sizeof(str_builtin)/sizeof(char*);
}

int number_builtin_bg() {
    return sizeof(str_builtin_bg)/sizeof(char*);
}

int launchProgram(char **args) {
    pid_t pid, wait_pid;
    int status;
    int background_check = checkRunBG(args);
    int builtin_bg = 0;

    //checking for built in commands to be run as bg
    for(int i=0 ; i < number_builtin_bg() ; ++i) {
        if(strcmp(args[0], str_builtin_bg[i])==0) {
            builtin_bg = 1;
        } 
    }

    if(builtin_bg == 1) {
        background_check = 2;
    }
    else if(background_check >= 0){
        args[background_check] = NULL;
    }

    pid = fork();

    if(pid == 0) {
        for(int i=0 ; i < number_builtin_bg() ; ++i) {
            if(strcmp(args[0], str_builtin_bg[i])==0) {
                return (*func_builtin_bg[i])(args, HOME_DIR);
            } 
        }
        if(execvp(args[0], args) == -1) {
            perror("Shell");
        }
        exit(EXIT_FAILURE);
    }
    else if (pid < 0) {
        perror("Shell");
    }
    else {
        if(background_check < 0) {
            do{
                wait_pid = waitpid(pid, &status, WUNTRACED);
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }
        else {
            printf("%d\n",pid);
            PROC_ARR[BG_PROC_COUNT].proc_id = pid;
            strcpy(PROC_ARR[BG_PROC_COUNT].proc_name, args[0]);
            ++BG_PROC_COUNT;
        }
    }
    return 1;
}

char *readInput(void) {
    char * input_line = NULL;
    size_t buffer_size = 0;
    getline(&input_line, &buffer_size, stdin);
    return input_line;
}

char **parseInput(char *input_line) {
    int buffer_size = TOKEN_BUFFER_SIZE;
    int position = 0;
    char **token_list = malloc(buffer_size * sizeof(char *));
    char *token;

    if(!token_list) {
        fprintf(stderr, "Allocation Error");
        exit(1); // EXIT_FAILURE
    }

    token = strtok(input_line, TOKEN_DELIMITERS);
    while(token != NULL) {
        token_list[position] = token;
        position++;

        if(position >= buffer_size) {
            buffer_size += TOKEN_BUFFER_SIZE;
            token_list = realloc(token_list, buffer_size * sizeof(char *));
            if(!token_list) {
                fprintf(stderr, "Allocation Error");
                exit(1); // EXIT_FAILURE
            }
        }
        token = strtok(NULL, TOKEN_DELIMITERS);
    }
    token_list[position] = NULL;
    return token_list;
}

char **parseInputSemiColon(char *input_line) {
    int buffer_size = TOKEN_BUFFER_SIZE;
    int position = 0;
    char **token_list = malloc(buffer_size * sizeof(char *));
    char *token;

    if(!token_list) {
        fprintf(stderr, "Allocation Error");
        exit(1); // EXIT_FAILURE
    }

    token = strtok(input_line, SEMICOLON_DELIMITERS);
    while(token != NULL) {
        token_list[position] = token;
        position++;

        if(position >= buffer_size) {
            buffer_size += TOKEN_BUFFER_SIZE;
            token_list = realloc(token_list, buffer_size * sizeof(char *));
            if(!token_list) {
                fprintf(stderr, "Allocation Error");
                exit(1); // EXIT_FAILURE
            }
        }
        token = strtok(NULL, SEMICOLON_DELIMITERS);
    }
    token_list[position] = NULL;
    return token_list; 
}

void checkBackgroundCompleted()
{
    for(int i=0 ; i<BG_PROC_COUNT ; ++i)
    {
        int retstat;
        if(waitpid(PROC_ARR[i].proc_id,&retstat,WNOHANG)>0)
        {
            printf("%s with process id %d exited normally",PROC_ARR[i].proc_name, PROC_ARR[i].proc_id);
            PROC_ARR[i].proc_id = -9999;
        }
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
}

void printPrompt(void) {
    char *working_directory;
    getcwd(CURRENT_DIR,1024);
    if(strstr(CURRENT_DIR, HOME_DIR) != NULL) {
        working_directory = strstr(CURRENT_DIR, HOME_DIR) + strlen(HOME_DIR);
    }
    else {
        working_directory = CURRENT_DIR;
    }
    USERNAME = getenv("LOGNAME");
    gethostname(HOSTNAME, 256);
    printf("\033[1;32m<\033[0m \033[1;36m%s\033[0m\033[1;31m@\033[0m\033[01;33m%s\033[0m : \033[1;35m~%s\033[0m \033[1;32m>\033[0m  ", USERNAME, HOSTNAME, working_directory);
}



int b_ls(char **args, char *home_directory) {
    return 1;
}
