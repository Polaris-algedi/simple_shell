#include "shell.h"

/**
 * *_strcpy - a function that copies the string pointed to by src,
 * including the terminating null byte (\0), to the buffer pointed to by dest
 * @dest: pointer to a char array
 * @src: pointer to a string
 * Return: *dest
 */
char *_strcpy(char *dest, char *src)
{
	int count = 0;

	while (*src)
	{
		*dest = *src;
		count++;
		src++;
		dest++;
	}
	*dest = '\0';
	return (dest - count);
}

/**
 * _strcmp - a function that compares two strings
 * @s1: This is the first string to be compared
 * @s2: This is the second string to be compared
 * Return: difference between the first nonidentical char
 */
int _strcmp(char *s1, char *s2)
{
	int diff;

	while (*s2)
	{
		if (*s2 != *s1)
			break;
		s2++;
		s1++;
	}
	diff = *s1 - *s2;
	return (diff);
}

/**
 * _strlen - a function that returns the length of a string.
 * @s: pointer to a string.
 * Return: the length of the string.
 */

int _strlen(char *s)
{
	if (!*s)
		return (0);
	return (1 + _strlen(++s));
}

/**
 *  _strdup - a function that returns a pointer to a newly
 *  allocated space in memory
 *  @str: string to copy
 * Return: pointer to the first character of the array
 */

char *_strdup(char *str)
{
	char *s;
	int len, i;

	if (str == NULL)
		return (NULL);
	len = _strlen(str);
	s = malloc(sizeof(*s) * len + 1);
	if (s == NULL)
		return (NULL);
	for (i = 0; i <= len; i++)
		s[i] = str[i];
	return (s);
}

/**
 * ignore_comment - a function that finds comment special character
 * and replace it with terminating null.
 * @line: string to check
 */
void ignore_comment(char *line)
{
	int i;

	for (i = 0; line[i] != '\0'; i++)
	{
		if (i > 0 && line[i] == '#' && line[i - 1] != ' ')
			continue;
		if (line[i] == '#')
			line[i] = '\0';
	}
}
