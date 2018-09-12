#include "cd.h"

int b_cd(char **args, char *home_directory) {
    // if no argument is given after cd
    if(args[1] == NULL) {
        if(chdir(home_directory) != 0) {
            perror("Shell");
        }
        return 1;
    }

    // handling path relative to home
    if(args[1][0] == '~') {
        char rel_path[1024];
        char abs_path[2048];
        int i;
        for(i=1 ; args[1][i] != '\0' ;++i) rel_path[i-1] = args[1][i];
        rel_path[i] = '\0';
        if(strcmp(args[1],"~") == 0)
        {
            rel_path[0] = '\0';
        }
        strcpy(abs_path, home_directory);
        strcat(abs_path, rel_path);
        if(chdir(abs_path) != 0) {
            perror("Shell");
        }
        return 1;
    }

    // handling all other paths ?
    if(chdir(args[1]) != 0) {
        perror("Shell");
    }
    return 1;
}