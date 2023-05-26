#include "shell.h"

/**
 * array_To_SLL - Copy each environment variable to the new linked list
 * Return:  head of linked list
*/
list_t *array_To_SLL(void)
{
	int i;
	size_t var_len;
	list_t *new = NULL, *head = NULL, *prev = NULL;

	for (i = 0; environ[i] != NULL; i++)
	{
		new = malloc(sizeof(list_t));
		if (new == NULL)
		{
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
 * Return: Nothing
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
/**
 *  SL_ListToArray - returns an array of strings
 *  @head: first node on the list
 * Return: NULL or array
*/
char **SL_ListToArray(const list_t *head)
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
	array = malloc((count + 1) * sizeof(char *));
	if (array == NULL)
	{
		perror("Memory allocation failed");
		exit(EXIT_FAILURE);
	}
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

/**
 * delete_node_at_index - deletes the node at index index of
 * a listint_t linked list
 * @head: double pointer to the listint_t list
 * @index: index of the node that should be deleted. Index starts at 0
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
/**
 * _unsetenv - remove an environment var.
 * @name: variable name
 * @head: head of a single linked list
 * Return: 0
 */
int _unsetenv(char *name, list_t **head)
{
	int node_index;

	node_index = check_name(name);
	if (node_index > 0)
	{
		delete_node_at_index(head, node_index);
		free(environ);
		environ = SL_ListToArray(*head);
	}
	return (0);
}
