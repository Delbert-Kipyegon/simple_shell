#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024
#define MAX_ARG_SIZE 100

/**
 * read_command - Reads and parses the user command.
 * @cmd: The command.
 * @args: List of arguments.
 * Return: Void.
 */
void read_command(char *cmd, char **args)
{
char *input = malloc(MAX_INPUT_SIZE);
int argcount = 0;
char *token;

fgets(input, MAX_INPUT_SIZE, stdin);
token = strtok(input, " \n\t");
strcpy(cmd, token);

while (token != NULL)
{
args[argcount] = token;
token = strtok(NULL, " \n\t");
argcount++;
}

args[argcount] = NULL;
free(input);
}

/**
 * main - Main loop for the shell.
 * Return: Always 0.
 */
int main(void)
{
char *args[MAX_ARG_SIZE];
char cmd[MAX_INPUT_SIZE];
pid_t pid;

while (1)
{
printf("> ");
read_command(cmd, args);

pid = fork();
if (pid == 0)  /* Child process */
{
execvp(cmd, args);
perror("Error executing command");
exit(EXIT_FAILURE);
}
else if (pid < 0)
{
perror("Fork failed");
exit(EXIT_FAILURE);
}
else
{
wait(NULL);
}
}

return (0);
}

