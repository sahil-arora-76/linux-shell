#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include<sys/utsname.h>
#include <stdlib.h>
#define BUFFERSIZE 100
#define GREEN "\033[0;32m"
#define NOCLR "\033[0m"
#define BLUE "\033[0;34m"

char *inbuild[] = { "cd", "help", "exit", "sysinfo" }; 

char *help =  "----------------------------\n"
"A Linux Terminal Made By notadevps\n"
"refer https://github.com/notadevps for more info\n" 
"built in commands -:\n" 
"cd - change directory\n"
"exit - exit from shell\n"
"help - all built in commands\n"
"sysinfo - system information\n"

"----------------------------\n";

void error(const char*); 
int execute(char **args); 

char **parse(char *str) 
{
    int buffer = 64; 
    char **arr = malloc(sizeof(char) * buffer); 
    char *key; 
    const char *delim = " \t\r\n\a"; 
    if (!arr) error("malloc error");  
    key = strtok(str, delim); 
    int i = 0; 
    while(key != NULL) 
    {
        arr[i] = key;
        i++; 
        if (i >= buffer) {
            buffer += 64;
            arr = realloc(arr, buffer * sizeof(char*));
            if (!arr) error("allocation error"); 
        }
        key = strtok(NULL, delim);  
    }
    arr[i] = NULL;
    return arr;
}

int build_fn(char **args)
{
    if (strcmp(args[0], inbuild[0]) == 0) 
    {
        if (!args[1]) 
        {
            char name[50]; 
            getlogin_r(name, 50); 
            char str[100]; 
            snprintf(str, sizeof(str), "/home/%s/Desktop", name);
            args[1] = str;  
        }
        if (chdir(args[1]) == -1 ) 
        {
            fprintf(stderr, "no directory found\n"); 
            return -1;
        } 
    } else if (strcmp(args[0], inbuild[2]) == 0) 
    {
        exit(0); 
    } else if (strcmp(args[0], inbuild[1]) == 0) 
    {
        printf("%s", help);
        return -1;
    } else if (strcmp(args[0], inbuild[3]) == 0)
    {
        struct utsname buf1;
        if(uname(&buf1) != 0)
        {
            return 1;
        }
        printf("System Name = %s\n", buf1.sysname);
        printf("Node Name = %s\n", buf1.nodename);
        printf("Version = %s\n", buf1.version);
        printf("Release = %s\n", buf1.release);
        printf("Machine = %s\n", buf1.machine);
    }
    return 1;
}

int process_exec(char **args) 
{
    pid_t pid; 
    pid = fork(); 
    if (pid == 0)
    {
        int k = execvp(args[0], args); 
        if (k == -1) 
        {
            fprintf(stderr, "no such command found\n"); 
        }
    } else if (pid < 0)
    {
        fprintf(stderr, "child process erorr\n");
    } else 
    {
        wait(NULL); 
    }
    return 1;
}

int execute(char **args) 
{
    if (!args[0]) return 1; 
    for (int i = 0; i < 4; i++)
    {
        if (strcmp(args[0], inbuild[i]) == 0) 
        {
            return build_fn(args);
        }
    }
    process_exec(args);
    return 1;
}

void error(const char *s) 
{
    perror(s); 
    exit(1); 
}

char *read_line() 
{
    int current_buffer_size = 100; 
    char *text = NULL; 
    text = (char*)malloc(sizeof(char) * current_buffer_size); 
    if (!text) error("malloc error"); 
    char ch; 
    int size = 0; 
    while (ch != '\n')
    {
        if (size > (current_buffer_size - 1)) 
        {
            text = realloc(text, BUFFERSIZE); 
            if (!text) error("realloc error"); 
            current_buffer_size += BUFFERSIZE;
        }
        ch = getchar(); 
        text[size] = ch;
        size++;
    }
    text[size] = '\0';
    return text;
}


void get_info() 
{
    char hostname[100];
    gethostname(hostname, sizeof(hostname)); 
    char name[300]; 
    getlogin_r(name, 300); 
    char path[200]; 
    getcwd(path, sizeof(path));
    printf("%s%s@%s~%s%s%s%s%c ", GREEN, name, hostname, NOCLR, BLUE, path, NOCLR, '$'); 
}

int main(int argc, char **agrv) 
{
    system("clear"); 
    char *str = NULL;
    char **args = NULL;
    do 
    {
        get_info();
        str = read_line(); 
        args = parse(str); 
        execute(args); 
        free(str); 
        free(args);
    } while(1);
}