#include "shell.h"

/**
 * _myexit - exits the shell
 * @pop: parameter structure for prototype
 *  Return: exits with a given exit status
 *         (0) if pop.argv[0] != "exit"
 */
int _myexit(pops *pop)
{
	int exitcheck;

	if (pop->argv[1])  /* If there is an exit arguement */
	{
		exitcheck = _erratoi(pop->argv[1]);
		if (exitcheck == -1)
		{
			pop->status = 2;
			print_error(pop, "Illegal number: ");
			_eputs(pop->argv[1]);
			_eputchar('\n');
			return (1);
		}
		pop->err_num = _erratoi(pop->argv[1]);
		return (-2);
	}
	pop->err_num = -1;
	return (-2);
}

/**
 * _mycd - changes the current directory of the process
 * @pop: parameter structure for prototype
 *  Return: Always 0
 */
int _mycd(pops *pop)
{
	char *s, *dir, buffer[1024];
	int chdir_ret;

	s = getcwd(buffer, 1024);
	if (!s)
		_puts("TODO: >>getcwd failure emsg here<<\n");
	if (!pop->argv[1])
	{
		dir = _getenv(pop, "HOME=");
		if (!dir)
			chdir_ret = /* TODO: what should this be? */
				chdir((dir = _getenv(pop, "PWD=")) ? dir : "/");
		else
			chdir_ret = chdir(dir);
	}
	else if (_strcmp(pop->argv[1], "-") == 0)
	{
		if (!_getenv(pop, "OLDPWD="))
		{
			_puts(s);
			_putchar('\n');
			return (1);
		}
		_puts(_getenv(pop, "OLDPWD=")), _putchar('\n');
		chdir_ret = /* TODO: what should this be? */
			chdir((dir = _getenv(pop, "OLDPWD=")) ? dir : "/");
	}
	else
		chdir_ret = chdir(pop->argv[1]);
	if (chdir_ret == -1)
	{
		print_error(pop, "can't cd to ");
		_eputs(pop->argv[1]), _eputchar('\n');
	}
	else
	{
		_setenv(pop, "OLDPWD", _getenv(pop, "PWD="));
		_setenv(pop, "PWD", getcwd(buffer, 1024));
	}
	return (0);
}

/**
 * _myhelp - changes the current directory of the process
 * @pop: parameter structure for prototype
 *  Return: Always 0
 */
int _myhelp(pops *pop)
{
	char **arg_array;

	arg_array = pop->argv;
	_puts("help call works. Function not yet implemented \n");
	if (0)
		_puts(*arg_array); /* temp att_unused workaround */
	return (0);
}
