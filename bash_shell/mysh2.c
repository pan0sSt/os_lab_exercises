#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

void par(char *command, char **argv)
{
        while (*command != '\0') {
                while (*command == ' ' || *command == '\n' || *command == '\t')
                {
                        *command++ = '\0';
                }
                if (*command)
                {
                        *argv++ = command;
                }
                while (*command != '\0' && *command != ' ' && *command != '\n' &&  *command != '\t')
                {
                        command++;
                }
        }
        *argv = '\0';
}

void execute(char **argv)
{
        pid_t pid, wpid;
        int status;
        int ret;
        char cwd[1024];

        if (strcmp(argv[0], "exit") == 0)
        {
                exit(0);
        }
        else if (strcmp(argv[0], "cd") == 0)
        {
                ret=chdir(argv[1]);
                if (getcwd(cwd, sizeof(cwd)) != NULL)
                        fprintf(stdout, "Current working dir: %s\n", cwd);
                else
                        perror("getcwd()");
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
        char **argv = malloc(64 * sizeof(char*));
        while (1)
        {
                loop:
                printf("$ ");
                fgets( command, sizeof command, stdin );
                if (*command == '\0' || *command == ' ' || *command == '\n' || *command == '\t')
                {
                        goto loop;
                }
                par(command, argv);
                execute(argv);
        }
        free(argv);
}

