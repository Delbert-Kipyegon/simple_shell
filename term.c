#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

/**
* main - Entry point of the shell.
*
* Return: 0 on success, other values on failure.
*/
int main(void)
{
char *line = NULL;
size_t len = 0;
ssize_t read;
int is_interactive = isatty(STDIN_FILENO);
pid_t child_pid;
int status;
char *args[2];

while (1)
{
if (is_interactive)
printf("#cisfun$ ");
read = getline(&line, &len, stdin);
if (read == -1)
break;
line[read - 1] = '\0';

args[0] = line;
args[1] = NULL;

child_pid = fork();
if (child_pid == 0)
{
execvp(args[0], args);
exit(0);
}
else
{
wait(&status);
}
}
free(line);
return (0);
}

