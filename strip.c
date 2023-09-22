#include "main.h"

/**
 * strip - Remove leading and trailing whitespace from a string.
 * @s: The string to strip.
 *
 * Return: Pointer to the stripped string.
 */
char *strip(char *s)
{
char *end;
if (s == NULL)
return (NULL);
while (*s && isspace((unsigned char)*s))
s++;
end = s + strlen(s) - 1;
while (end > s && isspace((unsigned char)*end))
end--;
*(end + 1) = '\0';
return (s);
}

