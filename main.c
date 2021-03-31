#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

void print_prompt1(void)
{
    fprintf(stderr, "$ ");
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
        if (ch == '\n')
        {
            break;
        }


        ch = getchar();
        temp[length] = ch;
        length++;
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


void display()
{
    system("clear"); 
    printf("\n\n\n\n\n"); 
    printf("%s\n", "\t\t###########################################################"); 
    printf("%s\n", "\t\t\t\t\tMY TERMINAL "); 
    printf("%s\n", "\t\t###########################################################"); 
}

int main(int argc, char **argv)
{
    display(); 
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