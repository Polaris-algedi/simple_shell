#include "shell.h"

/**
 * free_all_at_once - free everything allocated
 */
void free_all_at_once(void)
{
	free_them_all("sd", 1, 1, string, environ);
	free_list(head);
	write(STDOUT_FILENO, "\n", 1);
	exit(0);
}

/**
 * sig_handler - handle signal
 * @num: number
 */
void sig_handler(__attribute__((unused)) int num)
{
	free_all_at_once();
}

/**
 * print_prompt - print the shell prompt
*/
void print_prompt(void)
{
	if (write(STDOUT_FILENO, "$ ", 2) < 0)
		exit(EXIT_FAILURE);
}

/**
 * free_them_all - free allocated memories
 * @fmt: pointer to a string
 * @n: number of strings to free
 * @m: number of double char pointers to free
*/
void free_them_all(const char *fmt, int n, int m, ...)
{
	va_list args;
	int i, j, k;

	va_start(args, m);

	for (i = 0; fmt[i] != '\0'; i++)
	{
		switch (fmt[i])
		{
			case 's':
			for (j = 0; j < n; j++)
				free(va_arg(args, char *));
			break;
			case 'd':
			for (k = 0; k < m; k++)
				free(va_arg(args, char **));
			break;
			default:
			perror("Invalid format.");
		}
	}
	va_end(args);
}
