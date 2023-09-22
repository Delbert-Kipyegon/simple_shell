#include "main.h"
/**
* main - Main function for a simple shell.
* Description: This shell can execute commands with arguments.
* Return: 0 on successful exit, error number otherwise.
*/
int main(void)
{	char *line = NULL;
size_t len = 0;
ssize_t read;
int is_interactive = isatty(STDIN_FILENO);
pid_t child_pid;
int status;
char *args[64], *token;
int index, exit_status = 0;
while (1)
{
if (is_interactive)
printf("#cisfun$ ");
read = getline(&line, &len, stdin);
if (read == -1)
{	free(line);
if (is_interactive)
printf("\n");
exit(exit_status);	}
line[read - 1] = '\0';
line = strip(line);
if (strcmp(line, "exit") == 0)
{	free(line);
exit(exit_status);	}
token = strtok(line, " ");
index = 0;
while (token != NULL)
{	args[index++] = token;
token = strtok(NULL, " ");	}
args[index] = NULL;
child_pid = fork();
if (child_pid == 0)
{
if (execvp(args[0], args) == -1)
{	fprintf(stderr, "%s: command not found\n", args[0]);
exit(127);	}	}
else
{	wait(&status);
if (WIFEXITED(status))
{	exit_status = WEXITSTATUS(status);	}	}	}
free(line);
return (exit_status);	}
