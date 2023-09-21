#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
char *line = NULL;
size_t len = 0;
ssize_t read;
int is_interactive = isatty(STDIN_FILENO);
while (1)
{
if (is_interactive)
printf("#cisfun$ ");
read = getline(&line, &len, stdin);
if (read == -1)
break;
line[read - 1] = '\0';
char *args[] = {line, NULL};
pid_t child_pid;
int status;
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

