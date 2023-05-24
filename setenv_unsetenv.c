#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdarg.h>
#include <limits.h>
typedef struct list_t
{
    char *env_var;
    struct list_t *next;
} list_t;


void free_them_all(const char *fmt, const unsigned int n, const unsigned int m, ...);
void execute(char **commands);
char *_getenv(char *name);
char *concatenate(char *str1, char *str2, char *delim);
int filter(char *command);
void filter_and_execute(char **commands, char **av);
void exit_shell(char **commands, char **av);
void print_env();
int exe_built_ins(char **commands, char **av);
int _isdigit(char *s);
long int exit_atoi(char *s);
int _unsetenv(char *name, list_t **head);
int delete_node_at_index(list_t **head, unsigned int index);
int _setenv(char *name, char *value, list_t **head);
int check_name(char *name);
list_t *get_node_at_index(list_t *head, unsigned int index);
list_t *add_node_end(list_t **head, char *str);
char** SL_ListToArray(const list_t* head);
void free_list(list_t *head);
list_t *array_To_SLL();
void non_interactive_mode_error(char **commands, char **av);
int _strlen(char *s);
char *_strdup(char *str);
char *_strcpy(char *dest, char *src);
int _strcmp(char *s1, char *s2);
void ignore_comment(char *line);



extern char **environ;
char *string, *line;
list_t *head;




void free_all_at_once()
{
    free_them_all("sd", 1, 1, string, environ);
    free_list(head);
    write(STDOUT_FILENO, "\n", 1);
    exit(0);

}

void sig_handler()
{
    free_all_at_once();

}
char *readln()
{
    /* Here you just have a pointer(string) that is pointing to nothing (actually garbage): */
    size_t buffer = 0;
    ssize_t chars_read;

    /* I fix the bug of segmentation Fault by initializing string to NULL*/
    string = NULL;
    chars_read = getline(&string, &buffer, stdin);
    if (chars_read == EOF)
    {
        /*perror("getline");*/
        free_them_all("sd", 1, 1, string, environ);
        free_list(head);
        exit(0);
    }
    if (string == NULL)
	{
		write(STDOUT_FILENO, "\n", 1);
		free(string);
		exit(-1);
	}
    /* get rid of new line char and replace it with the terminating null*/
    string[chars_read - 1] = '\0';
    return (string);
}


char **split(char *string, char *delim)
{
    char **str_array;
    char *token, *str_copy;
    size_t i, nbr_words = 0;

    str_copy = _strdup(string);
    token = strtok(str_copy, delim);
    for (i = 1; token != NULL; i++)
    {
        token = strtok(NULL, delim);
    }

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

void print_prompt()
{
    if (write(STDOUT_FILENO, "$ ", 2) < 0)
            exit(EXIT_FAILURE);
}


void shell(char **av)
{
    char **commands;
    head = array_To_SLL();
    
    /* I have to create environ's double to not get errors when freeing it in _setenv & _unsetenv */
    environ = SL_ListToArray(head);
    /*size_t i;*/
    

    signal(SIGINT, sig_handler);
    while (1)
    {
        /* print $ */
        if (isatty(STDIN_FILENO))
                print_prompt();


        /* read line */
        /* when i pass line as a prameter to readln nothing works,
        but when i return a string and save it in it, it works ????*/
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
        
        /* create a child pross to execute the program
        and keep the shell runnig*/
        if (exe_built_ins(commands, av) > 0)
        {
            filter_and_execute(commands, av);
        }
    
        /* free char pointers */
        free_them_all("sd", 1, 1, line, commands);
    }

}

void free_them_all(const char *fmt, const unsigned int n, const unsigned int m, ...)
{
	va_list args;
	unsigned int i, j, k;

	va_start(args, m);

    for (i = 0; fmt[i] != '\0'; i++)
    {
        switch(fmt[i])
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

char *concatenate(char *str1, char *str2, char *delim)
{
    int len1, len2, len3;
    char *dest = NULL;
    char* start = NULL;
    
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

void filter_and_execute(char **commands, char **av)
{
    char *get_env, *env_cpy, *path;
    char **path_dirs;
    struct stat st;
    int i;


    /* filter the command & look for the file & execute it */
    if (filter(commands[0]) == 0)
    {
        /* get path directories */
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
        {   /* non-interactive mode */
            if (!isatty(STDIN_FILENO))
            {
                non_interactive_mode_error(commands, av);
            }
            else  /* interactive mode */
            {
                write(STDERR_FILENO, "hsh: ", 5);
                write(STDERR_FILENO, commands[0], _strlen(commands[0]));
                write(STDERR_FILENO, ": command not found\n", 20);
            }

        }

        free_them_all("sd", 1, 1, env_cpy, path_dirs);
    }
    else
    {
        if (stat(commands[0], &st) == 0)
            execute(commands);
        else
        {
            /* non-interactive mode */
            if (!isatty(STDIN_FILENO))
            {
                non_interactive_mode_error(commands, av);
            }
            else  /* interactive mode */
            {
                write(STDERR_FILENO, "hsh: ", 5);
                write(STDERR_FILENO, commands[0], _strlen(commands[0]));
                write(STDERR_FILENO, ": ", 2);
                errno = ENOENT;
                perror("");
                free_them_all("sd", 1, 2, line, commands, environ);
                free_list(head);
            }

        }

    }

}

void non_interactive_mode_error(char **commands, char **av)
{
    write(STDERR_FILENO, av[0], _strlen(av[0]));
    write(STDERR_FILENO, ": 1: ", 5);
    write(STDERR_FILENO, commands[0], _strlen(commands[0]));
    write(STDERR_FILENO, ": not found\n", 12);
    free_them_all("sd", 1, 2, line, commands, environ);
    free_list(head);
}

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

long int exit_atoi(char* s)
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
                {
                    free_them_all("sd", 1, 2, line, commands, environ);
                    free_list(head);
                    exit(ext_vl);
                }
            }

            write(STDERR_FILENO, av[0], _strlen(av[0]));
            write(STDERR_FILENO, ": 1: exit: Illegal number: ", 27);
            write(STDERR_FILENO, commands[1], _strlen(commands[1]));
            write(STDERR_FILENO, "\n", 1);
            free_them_all("sd", 1, 2, line, commands, environ);
            free_list(head);
            exit(2);
        }
        else
        {   /* interactive mode */
            if (_isdigit(commands[1]) == 0)
            {
                ext_vl = exit_atoi(commands[1]);
                free_them_all("sd", 1, 2, line, commands, environ);
                free_list(head);
                exit(ext_vl);
            }

            write(STDERR_FILENO, "hsh: exit: ", 11);
            write(STDERR_FILENO, commands[1], _strlen(commands[1]));
            write(STDERR_FILENO, ": numeric argument required", 27);
            free_them_all("sd", 1, 2, line, commands, environ);
            free_list(head);
            exit(2);
        }

    }
    else
    {
        free_them_all("sd", 1, 2, line, commands, environ);
        free_list(head);
        exit(ext_vl);
    }

}

void print_env()
{
    int i, len;

    for (i = 0; environ[i] != NULL; i++)
    {
        len = _strlen(environ[i]);
        write(STDOUT_FILENO, environ[i], len);
        write(STDOUT_FILENO, "\n", 1);
    }
        
}

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

/* all functions for _setenv and _unsetenv */
list_t *array_To_SLL()
{
    int i;
    size_t var_len;
    list_t *new = NULL, *head = NULL, *prev = NULL;
    
    /* Copy each environment variable to the new linked list */
    for (i = 0; environ[i] != NULL; i++)
    {
        new = malloc(sizeof(list_t));
        if (new == NULL)
        {
            /* Memory allocation failed, clean up the already copied variables */
            free_list(head);
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }
        if (i == 0)
            head = new;
        var_len = _strlen(environ[i]) + 1;  /* +1 for null-terminator */
        new->env_var = malloc(var_len);
        if (new->env_var == NULL)
        {
            /* Memory allocation failed, clean up the already copied variables */
            free_list(head);
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }
        _strcpy(new->env_var, environ[i]);
        /* Null-terminate the copied linked list */
        new->next = NULL;
        
        if (prev != NULL)
            prev->next = new;

        prev = new;
        
    }

    return (head);
}

/**
 * free_list - frees a linked list
 * @head: pointer to the list_t list to be freed
 */
void free_list(list_t *head)
{
	list_t *ptr_holder;

	while (head)
	{
		ptr_holder = head->next;
		free(head->env_var);
		free(head);
		head = ptr_holder;
	}
}


char** SL_ListToArray(const list_t* head)
{
    size_t count = 0, i;
    const list_t *current = head;
    char **array = NULL;
    
    if (head == NULL)
        return (NULL);
    /* Count the number of nodes in the linked list */
    while (current != NULL)
    {
        count++;
        current = current->next;
    }

    /* Allocate memory for the string array */
    array = malloc((count + 1) * sizeof(char *));
    if (array == NULL)
    {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    /* Copy each environment variable to the string array */
    current = head;
    for (i = 0; current != NULL; i++)
    {
        array[i] = current->env_var;
        current = current->next;
    }
    /* Null-terminate the string array */
    array[count] = NULL;  

    return (array);
}

void freeEnvironCopy(char **copied_environ)
{
    int i;
    /* Free the memory allocated for the copied environment variable array */
    if (copied_environ == NULL)
        return;

    for (i = 0; copied_environ[i] != NULL; i++)
        free(copied_environ[i]);

    free(copied_environ);
}

/**
 * add_node_end - adds a new node at the end of a linked list
 * @head: double pointer to the list_t list
 * @str: string to add in the new node
 *
 * Return: the address of the new element, or NULL if it failed
 */
list_t *add_node_end(list_t **head, char *str)
{
	list_t *temp = *head;
	list_t *new;


	new = (list_t *)malloc(sizeof(list_t));
	if (!new)
		return (NULL);

	new->env_var = str;
	new->next = NULL;

	if (!*head)
	{
		*head = new;
		return (new);
	}

	while (temp->next)
		temp = temp->next;
	temp->next = new;
	return (new);
}

/**
 * get_nodeint_at_index -  returns the nth node of a listint_t linked list
 * @head: pointer to the listint_t list
 * @index:  index of the node, starting at 0
 *
 * Return: the nth node of a listint_t linked list, or NULL
 * if the node does not exist
 */
list_t *get_node_at_index(list_t *head, unsigned int index)
{
	unsigned int i;

	for (i = 0; i < index; i++)
	{
		if (!head)
			return (NULL);
		head = head->next;
	}
	return (head);
}

int check_name(char *name)
{
    int i, j;
    
    if (name == NULL)
        return (-1);
        
    for (i = 0; environ[i] != NULL; i++)
    {
        for (j = 0; name[j] != '\0'; j++)
        {
            if (name[j] != environ[i][j])
                break;
        }
        
        if (environ[i][j] == '=' && name[j] == '\0')
            return (i);
        
    }
    return (-1);
}

int _setenv(char *name, char *value, list_t **head)
{
    list_t *node;
    char *env_var;
    int node_index;
    
    if (name == NULL || value == NULL)
    {
        perror("Setenv");
        return (-1);
    }
    /* if name does not exist */
    node_index = check_name(name);
    env_var = concatenate(name, value, "=");
    if (node_index == -1)
    {
        add_node_end(head, env_var);
        free(environ);
        environ = SL_ListToArray(*head);
        return (0);
    }
    else
    {
        /* if name exist */
        node = get_node_at_index(*head, node_index);
        free(node->env_var);
        node->env_var = env_var;
        free(environ);
        environ = SL_ListToArray(*head);
        return (0);
    }
    return (-1);
}


/**
 * delete_nodeint_at_index - deletes the node at index index of
 * a listint_t linked list
 * @head: double pointer to the listint_t list
 * @index: index of the node that should be deleted. Index starts at 0
 *
 * Return: 1 if it succeeded, -1 if it failed
 */
int delete_node_at_index(list_t **head, unsigned int index)
{
	unsigned int i;
	list_t *element_n_1, *temp;

	if (head == NULL || *head == NULL)
		return (-1);

	if (index == 0)
	{
		temp = (*head)->next;
		free(*head);
		*head = temp;

		return (1);
	}

	element_n_1 = *head;
	for (i = 0; i < index - 1; i++)
	{
		if (!element_n_1)
			return (-1);
		element_n_1 = element_n_1->next;
	}


	temp = element_n_1->next;
	element_n_1->next = temp->next;
	free(temp);

	return (1);
}


int _unsetenv(char *name, list_t **head)
{
    int node_index;
    
    if (name == NULL)
    {
        perror("Unsetenv");
        return (-1);
    }
    node_index = check_name(name);
    if (node_index > 0)
    {
        delete_node_at_index(head, node_index);
        free(environ);
        environ = SL_ListToArray(*head);
    }
    return (0);
}

int main(__attribute__((unused)) int ac, char **av)
{
    shell(av);
    return (0);
}

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
 *
 * @s1: This is the first string to be compared
 * @s2: This is the second string to be compared
 *
 * Return: difference between the first nonidentical char
 */
int _strcmp(char *s1, char *s2)
{
	int diff;

	while (*s2)
	{
		if (*s2 != *s1)
		{
			break;
		}
		s2++;
		s1++;
	}
	diff = *s1 - *s2;

	return (diff);
}

/**
 * _strlen - a function that returns the length of a string.
 *
 * @s: pointer to a string.
 *
 * Return: the length of the string.
 *
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
 *
 * @str: string to copy
 *
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

void ignore_comment(char *line)
{
    int i;
    
    for (i = 0; line[i] != '\0'; i++)
    {
        if (line[i] == '#')
            line[i] = '\0';
    }
}
