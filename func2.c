#include "shell.h"

/**
 * split - creates token after getting a line
 * @string: the string to parse
 * @delim: delimiter
 * Return: array of string
*/
char **split(char *string, char *delim)
{
	char **str_array;
	char *token, *str_copy;
	size_t i, nbr_words = 0;

	str_copy = _strdup(string);
	token = strtok(str_copy, delim);
	for (i = 1; token != NULL; i++)
		token = strtok(NULL, delim);

	free(str_copy);
	nbr_words = i;
	str_array = malloc(sizeof(char *) * nbr_words);
	if (str_array == NULL)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	token = strtok(string, delim);
	for (i = 0; token != NULL; i++)
	{
		str_array[i] = token;
		token = strtok(NULL, delim);
	}
	str_array[i] = NULL;
	return (str_array);
}

/**
 * execute - execute commands
 * @commands: command line arguments
 * Return: Nothing.
*/
void execute(char **commands)
{
	pid_t child_pid;
	int status;

	child_pid = fork();
	if (child_pid == -1)
	{
		perror("Error:");
		free_them_all("sd", 1, 2, line, commands, environ);
		free_list(head);
		exit(EXIT_FAILURE);
	}
	/* wait for the child pross to finish */
	if (child_pid == 0)
	{
		/* execute the command */
		if (execve(commands[0], commands, environ) == -1)
		{
			perror("./hsh");
			free_them_all("sd", 1, 2, line, commands, environ);
			free_list(head);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		/* wait for the parent pross */
		wait(&status);
	}
}

/**
 * _getenv - gets an envirenment variable value.
 * @name: variable name
 * Return: pointer to the value or NULL.
 */
char *_getenv(char *name)
{
	int i, j;

	if (name == NULL)
		return (NULL);

	for (i = 0; environ[i] != NULL; i++)
	{
		for (j = 0; name[j] != '\0'; j++)
		{
			if (name[j] != environ[i][j])
			break;
		}
		if (environ[i][j] == '=' && name[j] == '\0')
			return (&environ[i][j + 1]);
	}
	return (NULL);
}
/**
 * concatenate - concatenate two string
 * @str1: first string
 * @str2: second string
 * @delim: delimiter
 * Return: final result or NULL
*/
char *concatenate(char *str1, char *str2, char *delim)
{
	int len1, len2, len3;
	char *dest = NULL;
	char *start = NULL;

	if (str1 == NULL || str2 == NULL || delim == NULL)
		return (NULL);
	/* _strlen should be replaced*/
	len1 = _strlen(str1);
	len2 = _strlen(str2);
	len3 = _strlen(delim);
	/* create the exact memory for both strings */
	dest = malloc(sizeof(char *) * (len1 + len2 + len3 + 1));
	if (dest == NULL)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	start = dest;
	while (*str1 != '\0')
	*dest++ = *str1++;
	while (*delim != '\0')
	*dest++ = *delim++;
	while (*str2 != '\0')
	*dest++ = *str2++;
	*dest = '\0';
	return (start);
}

/**
 * exe_built_ins - executes builtins
 * @commands: command line arguments.
 * @av: array of string pointers
 * Return:0 or 1
*/
int exe_built_ins(char **commands, char **av)
{
	if (_strcmp(commands[0], "exit") == 0)
	{
		exit_shell(commands, av);
	}
	if (_strcmp(commands[0], "env") == 0)
	{
		print_env();
		return (0);
	}
	if (_strcmp(commands[0], "setenv") == 0)
	{
		_setenv(commands[1], commands[2], &head);
		return (0);
	}
	if (_strcmp(commands[0], "unsetenv") == 0)
	{
		_unsetenv(commands[1], &head);
		return (0);
	}
	return (1);
}
