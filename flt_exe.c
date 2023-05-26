#include "shell.h"

/**
 * filter - filter the command
 * @command: the command.
 * Return: 0 or 1
*/
int filter(char *command)
{
	int i;

	for (i = 0; command[i] != '\0'; i++)
	{
		if (command[i] == '/')
		return (1);
	}
	return (0);
}

/**
 * filter_and_execute - filter the command & look for the file & execute it
 * @commands: command line arguments
 * @av:array of string pointers.
 * Return: Nothing
 */

void filter_and_execute(char **commands, char **av)
{
	char *get_env, *env_cpy, *path, **path_dirs;
	struct stat st;
	int i;

	if (filter(commands[0]) == 0)
	{
		get_env = _getenv("PATH");
		env_cpy = _strdup(get_env);
		path_dirs = split(env_cpy, ":");

		for (i = 0; path_dirs[i] != NULL; i++)
		{
			path = concatenate(path_dirs[i], commands[0], "/");
			if (stat(path, &st) == 0)
			{
				commands[0] = path;
				execute(commands);
				break;
			}
			free(path);
		}
		if (path_dirs[i] != NULL)
			free(path);
		else
			command_not_found(commands, av);
		free_them_all("sd", 1, 1, env_cpy, path_dirs);
	}
	else
	{
		if (stat(commands[0], &st) == 0)
			execute(commands);
		else
		{
			if (!isatty(STDIN_FILENO))
				non_interactive_mode_error(commands, av);
			else
				interactive_mode_error(commands);
		}
	}
}

/**
 * non_interactive_mode_error - handle error in non_interactive mode
 * @commands: command line arguments
 * @av: array of string pointers
*/
void non_interactive_mode_error(char **commands, char **av)
{
	write(STDERR_FILENO, av[0], _strlen(av[0]));
	write(STDERR_FILENO, ": 1: ", 5);
	write(STDERR_FILENO, commands[0], _strlen(commands[0]));
	write(STDERR_FILENO, ": not found\n", 12);
	free_them_all("sd", 1, 2, line, commands, environ);
	free_list(head);
}

/**
 * interactive_mode_error - handle error in interactive mode
 * @commands: command line arguments
*/
void interactive_mode_error(char **commands)
{
	write(STDERR_FILENO, "hsh: ", 5);
	write(STDERR_FILENO, commands[0], _strlen(commands[0]));
	write(STDERR_FILENO, ": ", 2);
	errno = ENOENT;
	perror("");
}

/**
 * command_not_found - prints the right comment in the interactive and
 * non_interactive modes
 * @commands: command line arguments
 * @av: array of string pointers
 */
void command_not_found(char **commands, char **av)
{
	if (!isatty(STDIN_FILENO))
		non_interactive_mode_error(commands, av);
	else
	{
		write(STDERR_FILENO, "hsh: ", 5);
		write(STDERR_FILENO, commands[0], _strlen(commands[0]));
		write(STDERR_FILENO, ": command not found\n", 20);
	}
}
