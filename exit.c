#include "shell.h"

/**
 * _isdigit - checkes if a character is number or not
 * @s: input string
 * Return: 0 or -1
*/
int _isdigit(char *s)
{
	int i = 0;

	if (s[0] == '-')
		i++;

	for (; s[i] != '\0'; ++i)
	{
		if (!(s[i] >= '0' && s[i] <= '9'))
		return (-1);
	}
	return (0);
}
/**
 * exit_atoi - converting a string to an integer.
 * @s: input string
 * Return: return integer
 */
long int exit_atoi(char *s)
{
	long int res = 0;
	int sign = 1;
	int i = 0;

	/* If number is negative, then update sign */
	if (s[0] == '-')
	{
		sign = -1;
		i++;
	}
	for (; s[i] != '\0'; ++i)
	res = res * 10 + s[i] - '0';

	return (sign * res);
}

/**
 * exit_shell - exits the shell
 * @commands: command lines arguments.
 * @av: array of string pointers.
 * Return: Nothing.
*/
void exit_shell(char **commands, char **av)
{
	long int ext_vl = 0;

	if (commands[1] != NULL)
	{
		/* non-interactive mode */
		if (!isatty(STDIN_FILENO))
		{
			if (_isdigit(commands[1]) == 0)
			{
				ext_vl = exit_atoi(commands[1]);
				if (ext_vl > 0 && ext_vl <= INT_MAX)
					exit_free(ext_vl, commands);
			}
			write(STDERR_FILENO, av[0], _strlen(av[0]));
			write(STDERR_FILENO, ": 1: exit: Illegal number: ", 27);
			write(STDERR_FILENO, commands[1], _strlen(commands[1]));
			write(STDERR_FILENO, "\n", 1);
			exit_free(2, commands);
		}
		else
		{	/* interactive mode */
			if (_isdigit(commands[1]) == 0)
			{
				ext_vl = exit_atoi(commands[1]);
				exit_free(ext_vl, commands);
			}
			write(STDERR_FILENO, "hsh: exit: ", 11);
			write(STDERR_FILENO, commands[1], _strlen(commands[1]));
			write(STDERR_FILENO, ": numeric argument required", 27);
			exit_free(2, commands);
		}
	}
	else
		exit_free(ext_vl, commands);
}

/**
 * print_env - prints the current environement.
 * Return: Nothing.
*/
void print_env(void)
{
	int i, len;

	for (i = 0; environ[i] != NULL; i++)
	{
		len = _strlen(environ[i]);
		write(STDOUT_FILENO, environ[i], len);
		write(STDOUT_FILENO, "\n", 1);
	}
}

/**
 * exit_free - free line, commands, environ, and head
 * @nbr: exit status
 * @commands: commands to execute
 */
void exit_free(long int nbr, char **commands)
{
	free_them_all("sd", 1, 2, line, commands, environ);
	free_list(head);
	exit(nbr);
}
