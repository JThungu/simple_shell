#include "shell.h"

/**
 * _myenv - prints the current environment
 * @pop: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
int _myenv(pops *pop)
{
	print_list_str(pop->env);
	return (0);
}

/**
 * _getenv - gets the value of an environ variable
 * @pop: Structure containing potential arguments. Used to maintain
 * @name: env var name
 *
 * Return: the value
 */
char *_getenv(pops *pop, const char *name)
{
	list_t *node = pop->env;
	char *p;

	while (node)
	{
		p = starts_with(node->str, name);
		if (p && *p)
			return (p);
		node = node->next;
	}
	return (NULL);
}

/**
 * _mysetenv - Initialize a new environment variable,
 *             or modify an existing one
 * @pop: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int _mysetenv(pops *pop)
{
	if (pop->argc != 3)
	{
		_eputs("Incorrect number of arguements\n");
		return (1);
	}
	if (_setenv(pop, pop->argv[1], pop->argv[2]))
		return (0);
	return (1);
}

/**
 * _myunsetenv - Remove an environment variable
 * @pop: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int _myunsetenv(pops *pop)
{
	int i;

	if (pop->argc == 1)
	{
		_eputs("Too few arguements.\n");
		return (1);
	}
	for (i = 1; i <= pop->argc; i++)
		_unsetenv(pop, pop->argv[i]);

	return (0);
}

/**
 * populate_env_list - populates env linked list
 * @pop: Structure containing potential arguments.Used to maintain
 *          constant function prototype
 * Return: Always 0
 */
int populate_env_list(pops *pop)
{
	list_t *node = NULL;
	size_t i;

	for (i = 0; environ[i]; i++)
		add_node_end(&node, environ[i], 0);
	pop->env = node;
	return (0);
}
