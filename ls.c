#include "ls.h"

int check_hidden(char * file_name) 
{
    return file_name[0] == '.';
}

void set_flags(int *flags, char *arg) 
{
    for(int i=1 ; arg[i]!='\0' ; ++i)
    {
        switch(arg[i]) 
        {
            case 'l' : {
                flags[0] = 1;
                break;
            }

            case 'a' : {
                flags[1] = 1;
                break;
            }

            default : {
                break;
            }
        }
    }
}

void ls(char *dirname, int hidden)
{
    DIR *d;
    struct dirent *dir;
    d = opendir(dirname);
    struct dirent **scanned;
    int n = scandir(dirname, &scanned, NULL, alphasort);
    int i = 0;
    if(d == NULL)
    {
        if(errno == ENOTDIR) 
        {
            printf("%s\n",dirname); //incase of a file
        }
        else if (errno == ENOENT)
        {
            perror("Shell");
        }
    }
    else
    {
        printf("Listing : %s\n",dirname);
        if(hidden == 0)
        {
            while(i<n)
            {
                if(!check_hidden(scanned[i]->d_name))
                {
                    printf("%s\n",scanned[i]->d_name);
                }
                free(scanned[i]);
                ++i;
            }
        }
        else
        {
            while(i<n)
            {
                printf("%s\n",scanned[i]->d_name);
                free(scanned[i]);
                ++i;
            }
        }
    }
    printf("\n");
}

void getstat(char *filename, char *dirname)
{
    char resolved_path[1000];
    strcpy(resolved_path,dirname);
    strcat(resolved_path,"/");
    strcat(resolved_path,filename);
    struct stat info;
    char file_permissions[] = "----------";
    if(lstat(resolved_path, &info) != 0)
    {
        perror("Shell");
    }
    struct passwd *pw = getpwuid(info.st_uid); // pw->pw_name
    struct group  *gr = getgrgid(info.st_gid); // gr->gr_name
    file_permissions[0] = (S_ISDIR(info.st_mode))? 'd' : '-';
    file_permissions[1] = (info.st_mode & S_IRUSR)? 'r' : '-';
    file_permissions[2] = (info.st_mode & S_IWUSR)? 'w' : '-';
    file_permissions[3] = (info.st_mode & S_IXUSR)? 'x' : '-';
    file_permissions[4] = (info.st_mode & S_IRGRP)? 'r' : '-';
    file_permissions[5] = (info.st_mode & S_IWGRP)? 'w' : '-';
    file_permissions[6] = (info.st_mode & S_IXGRP)? 'x' : '-';
    file_permissions[7] = (info.st_mode & S_IROTH)? 'r' : '-';
    file_permissions[8] = (info.st_mode & S_IWOTH)? 'w' : '-';
    file_permissions[9] = (info.st_mode & S_IXOTH)? 'x' : '-';
    char t[100] = "";
    strftime(t, 100, "%b  %d %H:%M", localtime( &info.st_mtime));
    printf("%s\t%hu\t%s\t%s\t%lld\t\t%s\t%s\n",file_permissions,info.st_nlink,pw->pw_name,gr->gr_name,info.st_size,t,filename);
}

void lls(char *dirname, int hidden)
{
    
    DIR *d;
    struct dirent *dir;
    d = opendir(dirname);
    struct dirent **scanned;
    int n = scandir(dirname, &scanned, NULL, alphasort);
    int i = 0;
    if(d == NULL)
    {
        if(errno == ENOTDIR) 
        {
            printf("%s\n",dirname); //incase of a file
        }
        else if (errno == ENOENT)
        {
            perror("Shell");
        }
    }
    else
    {
        printf("Long Listing : %s\n",dirname);
        if(hidden == 0)
        {
            while(i<n)
            {
                if(!check_hidden(scanned[i]->d_name))
                {
                    getstat(scanned[i]->d_name, dirname);
                }
                free(scanned[i]);
                ++i;
            }
        }
        else
        {
            while(i<n)
            {
                getstat(scanned[i]->d_name, dirname);
                free(scanned[i]);
                ++i;
            }
        }
        free(scanned);
    }
    printf("\n");
}


int b_ls(char **args, char *home_directory) 
{
    int flag_set = 0;
    int read_start = 1; 
    int flag_bits[] = {0,0}; 
    char *arg = args[1]; 

    if(arg == NULL) read_start = 1; 

    int arg_count = 1;
    while(arg != NULL) 
    {
        if(arg[0] == '-')
        {
            if(strcmp(arg,"-") == 0) break;
            ++read_start;
            flag_set = 1;
            set_flags(flag_bits, arg);
            arg_count++;
            arg = args[arg_count];   
        }
        else
        {
            arg = NULL;
        }
    }

    arg = args[read_start];

    if(arg == NULL)
    {
        if(flag_bits[0] == 1)
            lls(".", flag_bits[1]);
        else
            ls(".", flag_bits[1]);
        return 1;
    }

    while(arg != NULL) 
    {
        if(arg[0] == '~')
        {
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
            if(flag_bits[0] == 1)
                lls(abs_path, flag_bits[1]);
            else
                ls(abs_path, flag_bits[1]);    

        }
        else 
        {
            if(flag_set == 0)
            {
                ls(arg, 0);
            }
            else if(flag_set == 1)
            {
                if(flag_bits[0] == 1)
                {
                    lls(arg, flag_bits[1]);
                }
                else if(flag_bits[0] == 0)
                {
                    ls(arg, flag_bits[1]);
                }
            }
        }
        ++read_start;
        arg = args[read_start];
    }
    return 1;
}