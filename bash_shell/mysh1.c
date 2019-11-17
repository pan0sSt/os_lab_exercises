#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void par(char *command, char **argv)
{
        while (*command != '\0') {
                while (*command == ' ' || *command == '\n')
                {
                        *command++ = '\0';
                }
                if (*command)
                {
                        *argv++ = command;
                }
                while (*command != '\0' && *command != ' ' && *command != '\n')
                {
                        command++;
                }
        }
        *argv = NULL;
}

void execute(char **argv)
{
        pid_t pid, wpid;
        int status;
        if (strcmp(argv[0], "exit") == 0)
        {
                exit(0);
        }
        else
        {
                pid = fork();
                if (pid == 0)
                {
                        if (execvp(*argv, argv) == -1)
                        {
                                perror("ERROR");
                        }
                        exit(EXIT_FAILURE);
                }
                else if (pid < 0)
                {
                        perror("ERROR");
                }
                else
                {
                        do
                        {
                        wpid = waitpid(pid, &status, WUNTRACED);
                        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
                }
        }
}

void main(void)
{
        char command[100];
        char *argv[10];
        while (1)
        {
        		loop:
                printf("$ ");
                fgets( command, sizeof command, stdin );
                if (*command == '\0' || *command == ' ' || *command == '\n')
                {
                        goto loop;
                }
                par(command, argv);
                execute(argv);
        }
}

