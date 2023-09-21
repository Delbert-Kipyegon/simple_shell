#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

/**
* strip - Removes leading and trailing whitespace from a string.
* @s: Input string to be stripped.
*
* Return: Pointer to the stripped string.
*/
char *strip(char *s)
{
char *end;
while (*s && isspace(*s))
s++;
end = s + strlen(s) - 1;
while (end > s && isspace(*end))
end--;
*(end + 1) = '\0';
return s;
}

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
line = strip(line);

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

