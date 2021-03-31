#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

void print_prompt1(void)
{
    char hostname[100];
    gethostname(hostname, sizeof(hostname)); 
    char name[300]; 
    getlogin_r(name, 300); 
    printf("%s@%s%c ", name, hostname, '$'); 
}

void print_prompt2(void)
{
    fprintf(stderr, "> ");
}


char *read_cmd() 
{
    char *str;
    char ch = 'p';
    char temp[1024]; 
    int length = 0;  
    while (1)
    {
        ch = getchar();
        temp[length] = ch;
        length++;

        if (ch == '\n' )
        {
            if (temp[length - 2] == '\\') 
            {
                print_prompt2();
                continue;
            } else 
            {
                break;
            }
        }

    }
    temp[length] = '\0';
    str = malloc(sizeof(char) * length);
    int i = 0; 
    while(temp[i] != '\0') 
    {
        str[i] = temp[i];
        i++;         
    }
    str[i] = '\0';
    return str; 
}



int main(int argc, char **argv)
{
    system("clear");
    char *cmd;

    do
    {
        print_prompt1();

        cmd = read_cmd();

        if(!cmd)
        {
            exit(EXIT_SUCCESS);
        }

        if(cmd[0] == '\0' || strcmp(cmd, "\n") == 0)
        {
            free(cmd);
            continue;
        }

        if(strcmp(cmd, "exit\n") == 0)
        {
            free(cmd);
            break;
        }
        system(cmd);
        
        free(cmd);
        } while(1);

    return 0;
}