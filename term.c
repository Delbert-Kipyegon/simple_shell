#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

/**
 * tokenize_input - Tokenizes a string into an array of strings
 * @line: The string to tokenize
 *
 * Return: A null-terminated array of strings
 */
char **tokenize_input(char *line)
{
char **tokens = malloc(64 * sizeof(char *));
char *token;
int bufsize = 64, position = 0;

if (!tokens)
{
perror("Allocation error");
exit(EXIT_FAILURE);
}

token = strtok(line, " \t\r\n\a");
while (token != NULL)
{
tokens[position] = token;
position++;
if (position >= bufsize)
{
bufsize += 64;
tokens = realloc(tokens, bufsize * sizeof(char *));
if (!tokens)
{
perror("Allocation error");
exit(EXIT_FAILURE);
}
}
token = strtok(NULL, " \t\r\n\a");
}
tokens[position] = NULL;
return tokens;
}

/**
 * main - The main shell loop
 *
 * Return: Always 0 (Success)
 */
int main(void)
{
char *line = NULL;
size_t len = 0;
ssize_t read;
int is_interactive = isatty(STDIN_FILENO);
char **args;
pid_t child_pid;
int status;

while (1)
{
if (is_interactive)
printf("#cisfun$ ");
read = getline(&line, &len, stdin);
if (read == -1)
{
perror("getline error");
free(line);
exit(EXIT_FAILURE);
}
line[read - 1] = '\0';

args = tokenize_input(line);

child_pid = fork();
if (child_pid == 0)
{
if (execvp(args[0], args) == -1)
{
perror(args[0]);
}
exit(EXIT_FAILURE);
}
else if (child_pid < 0)
{
perror("Fork error");
}
else
{
do
{
waitpid(child_pid, &status, WUNTRACED);
} while (!WIFEXITED(status) && !WIFSIGNALED(status));
}
free(args);
}
free(line);
return (0);
}

