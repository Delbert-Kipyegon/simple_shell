#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>

/**
* strip - Strips leading and trailing whitespaces from a string.
* @s: String to be stripped.
* Return: A pointer to the stripped string.
*/
char *strip(char *s)
{
char *end;
if (s == NULL) return NULL;
while (*s && isspace((unsigned char)*s))
s++;
end = s + strlen(s) - 1;
while (end > s && isspace((unsigned char)*end))
end--;
*(end + 1) = '\0';
return s;
}

/**
* main - Main function for a simple shell.
* Description: This shell can execute commands with arguments.
* It has a simple prompt when in interactive mode.
* Return: 0 on successful exit, error number otherwise.
*/
int main(void)
{
char *line = NULL;
size_t len = 0;
ssize_t read;
int is_interactive = isatty(STDIN_FILENO);
pid_t child_pid;
int status;
char *args[64];
char *token;
int index;

while (1)
{
if (is_interactive)
printf("#cisfun$ ");
read = getline(&line, &len, stdin);
if (read == -1)
{
free(line);
if (is_interactive)
printf("\n");
exit(0);
}
line[read - 1] = '\0';
line = strip(line);

if (strcmp(line, "exit") == 0)
{
free(line);
exit(0);
}

token = strtok(line, " ");
index = 0;
while (token != NULL)
{
args[index++] = token;
token = strtok(NULL, " ");
}
args[index] = NULL;
child_pid = fork();
if (child_pid == 0)
{
if (execvp(args[0], args) == -1)
{
fprintf(stderr, "%s: command not found\n", args[0]);
exit(127);
}
}
else
{
wait(&status);
if (WIFEXITED(status))
{
int exit_status = WEXITSTATUS(status);
if (exit_status != 0)
exit(exit_status);
}
}
}
free(line);
return (0);
}

