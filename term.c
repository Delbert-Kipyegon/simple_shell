#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

#define BUFF_SIZE 1024

/**
 * sigint_handler - Handle Ctrl+C signal interrupt
 * @sig_num: signal number, unused
 *
 * Return: void
 */
void sigint_handler(int sig_num)
{
    (void)sig_num;
    printf("\nExiting shell due to Ctrl+C.\n");
    exit(EXIT_SUCCESS);
}

/**
 * read_line - Read command line input
 * Return: buffer containing command line input
 */
char *read_line(void)
{
    char *buffer = NULL;
    size_t buffsize = 0;

    if (getline(&buffer, &buffsize, stdin) == -1)
    {
        if (feof(stdin)) 
        {
            exit(EXIT_SUCCESS);
        }
        else
        {
            perror("read_line");
            exit(EXIT_FAILURE);
        }
    }

    return buffer;
}

/**
 * split_line - Splits a line into tokens
 * @line: Line to split
 * Return: array of tokens
 */
char **split_line(char *line)
{
    int bufsize = BUFF_SIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token;

    if (!tokens)
    {
        perror("split_line");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, " \t\r\n\a");
    while (token != NULL)
    {
        tokens[position] = token;
        position++;

        token = strtok(NULL, " \t\r\n\a");
    }
    tokens[position] = NULL;
    return tokens;
}

/**
 * shell_execute - Execute shell commands
 * @args: command arguments
 * Return: execution status
 */
int shell_execute(char **args)
{
    pid_t pid;
    extern char **environ;
    int status;

    if (args[0] == NULL)
    {
           return 1;
    }

    if (strcmp(args[0], "exit") == 0)
    {
        exit(EXIT_SUCCESS);
    }
    else if (strcmp(args[0], "env") == 0)
    {
        char **env = environ;
        while (*env)
        {
            printf("%s\n", *env);
            env++;
        }
        return 1;
    }
    else if (strcmp(args[0], "pwd") == 0)
    {
        char cwd[BUFF_SIZE];
        getcwd(cwd, sizeof(cwd));
        printf("%s\n", cwd);
        return 1;
    }

    pid = fork();
    if (pid == 0)
    {
       
        if (execvp(args[0], args) == -1)
        {
            perror("shell");
        }
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
              perror("shell");
    }
    else
    {
       
        do
        {
          waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

/**
 * main - Main shell loop
 * Return: exit status
 */
int main(void)
{
    char *line;
    char **args;

    
    signal(SIGINT, sigint_handler);

    do
    {
        printf("> ");
        line = read_line();
        args = split_line(line);
        shell_execute(args);

        free(line);
        free(args);
    } while (1);

    return EXIT_SUCCESS;
}

