#include "pinfo.h"

extern pid_t GLOBAL_PID;

int b_pinfo(char **args, char *home_directory)
{
    pid_t process_id = GLOBAL_PID;
    if(args[1] != NULL)
    {
        process_id = to_integer_p(args[1]);
    }
    char status_path[500], exectuable_path[500];
    sprintf(status_path,"/proc/%d/status",process_id);
    sprintf(exectuable_path,"/proc/%d/exe", process_id);
    if(!check_file_exists(status_path))
    {
        printf("Process with process id %d does not exist\n",process_id);
        return 1;
    }
    char link_path[1000];
    for(int i=0;i<1000;++i)link_path[i] = '\0';
    char status[256], vmsize[256];
    read_ith_line(status_path, 3, status);
    read_ith_line(status_path, 18, vmsize);
    printf("pid -- %d\n",process_id);
    printf("Process Status -- %s\n", status);
    printf("Virtual Memory -- %s\n", vmsize);    
    printf("Executable Path -- ");
    if(readlink(exectuable_path, link_path, 1000) == -1)
    {
        printf("No Executable path\n");
    }
    else
    {
        printf("%s\n", link_path);
    }
    return 1;
}

int read_ith_line(char * filename, int line_number, char * line)
{
    FILE* file = fopen(filename, "r");
    for(int i=0;i<line_number;++i)
    {
        fgets(line,256, file);
    }
    fclose(file);

    return 1;
}

int check_file_exists(const char * filename){
    FILE *file;
    if ((file = fopen(filename, "r"))){
        fclose(file);
        return 1;
    }
    return 0;
}

void to_string(char *str, int num)
{
    int i, rem, len = 0, n;
    n = num;
    while (n != 0)
    {
        len++;
        n /= 10;
    }
    for (i = 0; i < len; i++)
    {
        rem = num % 10;
        num = num / 10;
        str[len - (i + 1)] = rem + '0';
    }
    str[len] = '\0';
}

int to_integer_p(char str[])
{
    int len = strlen(str);
    int i, num = 0;
 
    for (i = 0; i < len; i++)
    {
        num = num + ((str[len - (i + 1)] - '0') * pow(10, i));
    }
 
   return num;
}