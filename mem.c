#include "shell.h"
/**
 * bfree - NULLs the address and frees memory
 * @ptr: address of the pointers
 *
 * Return: 1 if freed, otherwise 0.
 */
int bfree(void **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
		return (1);
	}
	return (0);
}
