#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>

/**
* strip - Strips leading and trailing whitespaces.
* @s: String to be stripped.
* 
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
* main - Main function of the shell.
* 
* Return: 0 on success, and other values on error.
*/
int main(void)
{
char *line = NULL;
size_t len = 0;
ssize_t read;
int is_interactive = isatty(STDIN_FILENO);
pid_t child_pid;
int status;
char *args[64];  /* Assume a maximum of 64 arguments for simplicity */
char *token;
int index;

while (1)
{
if (is_interactive)
printf("#cisfun$ ");
read = getline(&line, &len, stdin);
if (read == -1)
break;
line[read - 1] = '\0';
line = strip(line);

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
perror(args[0]);
}
exit(EXIT_FAILURE);
}
else
{
wait(&status);
}
}
free(line);
return (0);
}

