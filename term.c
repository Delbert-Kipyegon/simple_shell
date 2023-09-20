#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUF_SIZE 1024

/**
 * display_prompt - Displays the shell prompt.
 */
void display_prompt(void)
{
    write(STDOUT_FILENO, "$ ", 2);
}

/**
 * main - The main function for the simple shell.
 *
 * Return: 0 on success, 1 on failure.
 */
int main(void)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read_len;
    size_t newline_pos;
    pid_t pid;
    char *argv[2];

    display_prompt();
    read_len = getline(&line, &len, stdin);

    if (read_len == -1)
    {
        write(STDOUT_FILENO, "\n", 1);
        free(line);
        exit(0);
    }

    newline_pos = strcspn(line, "\n");
    line[newline_pos] = '\0';

    pid = fork();

    if (pid < 0)
    {
        perror("Fork failed");
        free(line);
        exit(1);
    }

    if (pid == 0)
    {
        argv[0] = line;
        argv[1] = NULL;

        if (execve(line, argv, NULL) == -1)
        {
            perror(line);
        }
        exit(EXIT_FAILURE);
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
    }

    free(line);
    return (0);
}

