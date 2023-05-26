#include "shell.h"

/**
 * get_environ - returns the string array copy of our environ
 * @pop: Structure containing potential arguments. Used to maintain
 *          constant function prototype
 * Return: Always 0
 */
char **get_environ(pops *pop)
{
	if (!pop->environ || pop->env_changed)
	{
		pop->environ = list_to_strings(pop->env);
		pop->env_changed = 0;
	}

	return (pop->environ);
}

/**
 * _unsetenv - Remove an environment variable
 * @pop: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: 1 on delete, 0 otherwise
 * @var: the string env var property
 */
int _unsetenv(pops *pop, char *var)
{
	list_t *node = pop->env;
	size_t i = 0;
	char *p;

	if (!node || !var)
		return (0);

	while (node)
	{
		p = starts_with(node->str, var);
		if (p && *p == '=')
		{
			pop->env_changed = delete_node_at_index(&(pop->env), i);
			i = 0;
			node = pop->env;
			continue;
		}
		node = node->next;
		i++;
	}
	return (pop->env_changed);
}

/**
 * _setenv - Initialize a new environment variable,
 *             or modify an existing one
 * @pop: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @var: the string env var property
 * @value: the string env var value
 *  Return: Always 0
 */
int _setenv(pops *pop, char *var, char *value)
{
	char *buf = NULL;
	list_t *node;
	char *p;

	if (!var || !value)
		return (0);

	buf = malloc(_strlen(var) + _strlen(value) + 2);
	if (!buf)
		return (1);
	_strcpy(buf, var);
	_strcat(buf, "=");
	_strcat(buf, value);
	node = pop->env;
	while (node)
	{
		p = starts_with(node->str, var);
		if (p && *p == '=')
		{
			free(node->str);
			node->str = buf;
			pop->env_changed = 1;
			return (0);
		}
		node = node->next;
	}
	add_node_end(&(pop->env), buf, 0);
	free(buf);
	pop->env_changed = 1;
	return (0);
}
