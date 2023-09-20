#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define PROMPT "#cisfun$ "
#define BUF_SIZE 1024

/**
 * display_prompt - Displays the shell prompt.
 */
void display_prompt(void)
{
write(STDOUT_FILENO, PROMPT, strlen(PROMPT));
}

/**
 * main - Entry point for the simple shell.
 *
 * Return: 0 on success, otherwise on failure.
 */
int main(void)
{
char *line = NULL;
size_t len = 0;
ssize_t read_len;
pid_t pid;

while (1)
{
display_prompt();

read_len = getline(&line, &len, stdin);

if (read_len == -1)
{
write(STDOUT_FILENO, "\n", 1);
free(line);
exit(0);
}

line[read_len - 1] = '\0';
pid = fork();

if (pid == -1)
{
perror("Error:");
continue;
}

if (pid == 0)
{
char *argv[2];
argv[0] = line;
argv[1] = NULL;


if (execve(line, argv, NULL) == -1)
{
perror("./shell");
exit(EXIT_FAILURE);
}
}
else
{
int status;
waitpid(pid, &status, 0);
}
}
free(line);
return (0);
}

