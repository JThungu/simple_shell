#include "shell.h"

/**
 * hsh - main shell loop
 * @pop: the parameter & return pop struct
 * @av: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int hsh(pops *pop, char **av)
{
	ssize_t r = 0;
	int builtin_ret = 0;

	while (r != -1 && builtin_ret != -2)
	{
		clear_pop(pop);
		if (interactive(pop))
			_puts("$ ");
		_eputchar(BUF_FLUSH);
		r = get_input(pop);
		if (r != -1)
		{
			set_pop(pop, av);
			builtin_ret = find_builtin(pop);
			if (builtin_ret == -1)
				find_cmd(pop);
		}
		else if (interactive(pop))
			_putchar('\n');
		free_pop(pop, 0);
	}
	write_history(pop);
	free_pop(pop, 1);
	if (!interactive(pop) && pop->status)
		exit(pop->status);
	if (builtin_ret == -2)
	{
		if (pop->err_num == -1)
			exit(pop->status);
		exit(pop->err_num);
	}
	return (builtin_ret);
}

/**
 * find_builtin - finds a builtin command
 * @pop: the parameter & return pop struct
 *
 * Return: -1 if builtin not found,
 *			0 if builtin executed successfully,
 *			1 if builtin found but not successful,
 *			-2 if builtin signals exit()
 */
int find_builtin(pops *pop)
{
	int i, built_in_ret = -1;
	builtin_table builtintbl[] = {
		{"exit", _myexit},
		{"env", _myenv},
		{"help", _myhelp},
		{"history", _myhistory},
		{"setenv", _mysetenv},
		{"unsetenv", _myunsetenv},
		{"cd", _mycd},
		{"alias", _myalias},
		{NULL, NULL}
	};

	for (i = 0; builtintbl[i].type; i++)
		if (_strcmp(pop->argv[0], builtintbl[i].type) == 0)
		{
			pop->line_count++;
			built_in_ret = builtintbl[i].func(pop);
			break;
		}
	return (built_in_ret);
}

/**
 * find_cmd - finds a command in PATH
 * @pop: the parameter & return pop struct
 *
 * Return: void
 */
void find_cmd(pops *pop)
{
	char *path = NULL;
	int i, k;

	pop->path = pop->argv[0];
	if (pop->linecount_flag == 1)
	{
		pop->line_count++;
		pop->linecount_flag = 0;
	}
	for (i = 0, k = 0; pop->arg[i]; i++)
		if (!is_delim(pop->arg[i], " \t\n"))
			k++;
	if (!k)
		return;

	path = find_path(pop, _getenv(pop, "PATH="), pop->argv[0]);
	if (path)
	{
		pop->path = path;
		fork_cmd(pop);
	}
	else
	{
		if ((interactive(pop) || _getenv(pop, "PATH=")
			|| pop->argv[0][0] == '/') && is_cmd(pop, pop->argv[0]))
			fork_cmd(pop);
		else if (*(pop->arg) != '\n')
		{
			pop->status = 127;
			print_error(pop, "not found\n");
		}
	}
}

/**
 * fork_cmd - forks a an exec thread to run cmd
 * @pop: the parameter & return pop struct
 *
 * Return: void
 */
void fork_cmd(pops *pop)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		/* TODO: PUT ERROR FUNCTION */
		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(pop->path, pop->argv, get_environ(pop)) == -1)
		{
			free_pop(pop, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
		/* TODO: PUT ERROR FUNCTION */
	}
	else
	{
		wait(&(pop->status));
		if (WIFEXITED(pop->status))
		{
			pop->status = WEXITSTATUS(pop->status);
			if (pop->status == 126)
				print_error(pop, "Permission denied\n");
		}
	}
}

