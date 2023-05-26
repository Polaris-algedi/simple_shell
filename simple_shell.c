#include "shell.h"
char *string, *line;
list_t *head;

/**
 * readln - gets line from input stream
 * Return: the line read
*/
char *readln(void)
{
	size_t buffer = 0;
	ssize_t chars_read;

	string = NULL;
	chars_read = getline(&string, &buffer, stdin);
	if (chars_read == EOF)
	{
		free_them_all("sd", 1, 1, string, environ);
		free_list(head);
		exit(0);
	}
	if (string == NULL)
	{
		write(STDOUT_FILENO, "\n", 1);
		free(line);
		exit(-1);
	}
	string[chars_read - 1] = '\0';
	return (string);
}

/**
 * shell - shell loop
 * @av: array of  string pointers
 * Return: Nothing
*/
void shell(char **av)
{
	char **commands;

	head = array_To_SLL();
	environ = SL_ListToArray(head);

	signal(SIGINT, sig_handler);
	while (1)
	{
		if (isatty(STDIN_FILENO))
			print_prompt();

		line = readln();

		ignore_comment(line);
		/* parse it */
		commands = split(line, " ");

		/* handle passing 0 words */
		if (commands[0] == NULL)
		{
			free_them_all("sd", 1, 1, line, commands);
			continue;
		}

		if (exe_built_ins(commands, av) > 0)
			filter_and_execute(commands, av);

		free_them_all("sd", 1, 1, line, commands);
	}
}

/**
 * main - main function
 * @ac: number of arguments
 * @av: array of string pointers
 * Return: 0
*/
int main(__attribute__((unused)) int ac, char **av)
{
	shell(av);
	return (0);
}
