#include "shell.h"

/**
 * freeEnvironCopy - Free the memory allocated for the copied envir. var.
 * @copied_environ: the copied environment variable array
 * Return: Nothing
*/
void freeEnvironCopy(char **copied_environ)
{
	int i;

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
 * get_node_at_index -  returns the nth node of a listint_t linked list
 * @head: pointer to the listint_t list
 * @index: index of the node, starting at 0
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

/**
 * check_name - check for environ. name
 * @name: name
 * Return: index  if success, -1 for failure.
*/
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

/**
 * _setenv - initialize a new environment var. or modify an existing one.
 * @name: variable name
 * @value: variable value
 * @head: head of a single linked list
 * Return: 0 or -1;
*/
int _setenv(char *name, char *value, list_t **head)
{
	list_t *node;
	char *env_var;
	int node_index;

	if (name == NULL || value == NULL)
		return (-1);
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
