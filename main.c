#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#define BUFFERSIZE 1024

void get_info(void)
{
    char hostname[100];
    gethostname(hostname, sizeof(hostname)); 
    char name[300]; 
    getlogin_r(name, 300); 
    printf("%s@%s%c ", name, hostname, '$'); 
}

int built_cd(char **args) 
{
    if (!args[0]) 
    {
        fprintf(stderr, "expected argument\n");
    }
    if (chdir(args[1]) == -1 ) 
    {
        fprintf(stderr, "no directory found\n"); 
        return -1;
    }
    return 1;
}

int built_exit() 
{
    exit(0);
}

char *read_line(void) 
{
    int pos = 0; 
    char ch;
    char *buffer = malloc(sizeof(char) * BUFFERSIZE); 
    while(ch != EOF && ch != '\n') 
    {
        ch = getchar(); 
        buffer[pos] = ch;
        pos++; 
    }
    buffer[pos + 1] = '\0';

    //TODO -> excedded the buffer reallocate
    return buffer;
}


int process_exec(char **args) 
{
    pid_t pid, wpid; 
    int status; 
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

int exec(char **arr)
{
    const char *built = "cd";  
    const char *built_e = "exit"; 
    int i; 
    if (!arr[0]) 
    {
        return 1;
    }
    if (strcmp(arr[0], built) == 0) 
    {
        return built_cd(arr); 
    } else if (strcmp(arr[0], built_e) == 0)
    {
        return built_exit(); 
    }

    return process_exec(arr);
}


char **split(char *line) 
{
    int buffer = 64; 
    char **arr = malloc(sizeof(char) * buffer); 
    char *key; 
    const char *delim = " \t\r\n\a"; 
    if (!arr) 
    {
        fprintf(stderr, "malloc error\n"); 
        exit(0); 
    }
    key = strtok(line, delim); 
    int i = 0; 
    while(key != NULL) 
    {
        arr[i] = key;
        i++; 
        if (i >= buffer) {
            buffer += 64;
            arr = realloc(arr, buffer * sizeof(char*));
            if (!arr) {
                fprintf(stderr, "lsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        key = strtok(NULL, delim);  
    }
    arr[i] = NULL;
    return arr;
}





int main() 
{
    
    char *line; 
    char **args; 
    int status;
    do 
    {
        get_info(); 
        line = read_line(); 
        args = split(line); 
        status = exec(args); 
    
        free(line); 
        free(args);
    } while(status);

}