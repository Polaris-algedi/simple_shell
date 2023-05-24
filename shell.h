#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdarg.h>
#include <limits.h>
extern char **environ;
/**
 * CU_int - Typedef for const unsigned int
*/
typedef const unsigned int CU_int;
/**
 * struct list_t - single linked list to store env_var.
 * @env_var: environement variables
 * @next: next node
*/
typedef struct list_t
{
char *env_var;
struct list_t *next;
} list_t;

void shell(char **av);
void print_prompt(void);
char *readln(void);
char **split(char *string, char *delim);
void execute(char **commands);
void free_them_all(const char *fmt, CU_int  n, CU_int m, ...);
void ignore_comment(char *line);
void sig_handler(__attribute__((unused)) int num);
char *_getenv(char *name);
char *concatenate(char *str1, char *str2, char *delim);
int filter(char *command);
void filter_and_execute(char **commands, char **av);
void exit_shell(char **commands, char **av);
void print_env(void);
int exe_built_ins(char **commands, char **av);
int _isdigit(char *s);
long int exit_atoi(char *s);
int _unsetenv(char *name, list_t **head);
int delete_node_at_index(list_t **head, unsigned int index);
int _setenv(char *name, char *value, list_t **head);
int check_name(char *name);
list_t *get_node_at_index(list_t *head, unsigned int index);
list_t *add_node_end(list_t **head, char *str);
char **SL_ListToArray(const list_t *head);
void free_list(list_t *head);
list_t *array_To_SLL(void);
void non_interactive_mode_error(char **commands, char **av);
int _strlen(char *s);
char *_strdup(char *str);
char *_strcpy(char *dest, char *src);
int _strcmp(char *s1, char *s2);
#endif
