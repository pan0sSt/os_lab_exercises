#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

void par(char *command, char **argv)
{
        int i;
        while (*command != '\0') {
                while (*command == ' ' || *command == '\n' || *command == '\t')
                {
                        *command++ = '\0';
                }
                if (*command)
                {
                        *argv++ = command;
                        i++;
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
                        perror("getcwd() error");
        }
        else
        {
                pid = fork();
                if (pid == 0)
                {

                        if (execvp(*argv, argv) == -1)
                        {
                                perror("execvp");
                        }
                        exit(EXIT_FAILURE);
                }
                else if (pid < 0)
                {
                        perror("fork");
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

void runpipe(int fds[], char **argv1, char **argv2)
{
        pid_t pid;
        pid = fork();
        if (pid == 0)
        {
//              close(fds[1]);
                dup2(fds[0], 0);
                close(fds[1]);
                if (execvp(*argv2, argv2) == -1)
                {
                        perror("execvp");
                }
                exit(EXIT_FAILURE);
        }
        else if (pid < 0)
        {
                perror("fork");
        }
        else
        {
//              close(fds[0]);
                dup2(fds[1], 1);
                close(fds[0]);
                if (execvp(*argv1, argv1) == -1)
                {
                        perror("execvp");
                }
                exit(EXIT_FAILURE);
        }
}

void pipe_parse_ex(char *command, char **argv)
{
        int i, max, k, status, br;
        char *argv1[100];
        char *argv2[100];
        for(i=0; i<=100; i++)
        {
                argv[i]=strsep(&command, " ");
                max++;
                if (argv[i]==NULL) break;
        }
        for(i=0; i<=max; i++)
        {
                if (strcmp(argv[i], "|") == 0)
                {
                        br = i;
                        for(k=0; k<i; k++)
                        {
                                argv1[k]=argv[k];
                        }
                        argv1[k+1] = NULL; //bad address
                        for(k=i+1; k<max; k++)
                        {
                                argv2[k]=argv[k];
                        }
//                      argv2[k+1] = NULL;
                        break;
                }
        }
        printf("Array1:\n");
        for(i=0; i<br+1; i++)
        {
                printf("%s\n", argv1[i]);
        }
        printf("Array2:\n");
        for(i=br+1; i<max-1; i++)
        {
                printf("%s\n", argv2[i]);
        }
        int fds[2];
        pipe(fds);
        pid_t pid, wpid;
        pid = fork();
        if (pid == 0)
        {
                runpipe(fds, argv1, argv2);
                exit(EXIT_FAILURE);
        }
        else if (pid < 0)
        {
                perror("fork");
        }
        else
        {
                do
                {
                        wpid = waitpid(pid, &status, WUNTRACED);
                } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }
}

void main(void)
{
        int k, pipe_flag;
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
                pipe_flag=0;
                for(k=0; k<=100; k++)
                {
                        if (command[k]=='|' && command[k-1]==' ' && command[k+1]==' ') // elegxos gia pipe
                        {
                                pipe_flag=1;
                        }
                        if (command[k]=='\n') break;
                }
                if (pipe_flag==1) //an uparxei pipe
                {
                        pipe_parse_ex(command, argv);
                }
                else if (pipe_flag==0) // an den uparxei pipe
                {
                        par(command, argv);
                        execute(argv);
                }
        }
        free(argv);
}

