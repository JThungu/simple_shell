#include "shell.h"

/**
 * clear_pop - initializes pops struct
 * @pop: parameter structure for prototype
 */
void clear_pop(pops *pop)
{
	pop->arg = NULL;
	pop->argv = NULL;
	pop->path = NULL;
	pop->argc = 0;
}

/**
 * set_pop - initializes pops struct
 * @pop: parameter structure for prototype
 * @av: argument vector
 */
void set_pop(pops *pop, char **av)
{
	int i = 0;

	pop->fname = av[0];
	if (pop->arg)
	{
		pop->argv = strtow(pop->arg, " \t");
		if (!pop->argv)
		{

			pop->argv = malloc(sizeof(char *) * 2);
			if (pop->argv)
			{
				pop->argv[0] = _strdup(pop->arg);
				pop->argv[1] = NULL;
			}
		}
		for (i = 0; pop->argv && pop->argv[i]; i++)
			;
		pop->argc = i;

		replace_alias(pop);
		replace_vars(pop);
	}
}

/**
 * free_pop - frees pops struct fields
 * @pop: parameter structure for prototype
 * @all: true if freeing all fields
 */
void free_pop(pops *pop, int all)
{
	ffree(pop->argv);
	pop->argv = NULL;
	pop->path = NULL;
	if (all)
	{
		if (!pop->cmd_buf)
			free(pop->arg);
		if (pop->env)
			free_list(&(pop->env));
		if (pop->history)
			free_list(&(pop->history));
		if (pop->alias)
			free_list(&(pop->alias));
		ffree(pop->environ);
			pop->environ = NULL;
		bfree((void **)pop->cmd_buf);
		if (pop->readfd > 2)
			close(pop->readfd);
		_putchar(BUF_FLUSH);
	}
}
