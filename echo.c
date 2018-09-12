#include "echo.h"

int b_echo(char **args, char *home_directory) {
    if (args[1] == NULL) {
        printf("\n"); //nothing entered after echo
        return 1;
    }

    int count = 1;
    while(args[count] != NULL) ++count;
    
    for(int i=1 ; i<count ; ++i)
    {
        if(args[i][0] == '$') {
            char * env_var = getenv(args[i]+1);
            if(env_var != NULL) args[i] = env_var;
        }
    }

    int largslen = strlen(args[count-1]);
    if(args[1][0] == '"' && args[count-1][largslen-1] == '"') {
        args[count-1][largslen-1] = '\0';
        args[1] = args[1] + 1;
    }

    int i=1;
    while(args[i] != NULL) {
        printf("%s ",args[i]);
        ++i;
    }
    printf("\n");
    return 1;
}