#include "../minishell.h"
#include <stdbool.h>
#include <stdio.h>

bool check_exit_value(char *val)
{
	while (*val)
		if (!ft_isdigit(*val))
			return (false);
	return (true); 
}


size_t  sizeof_args(char **args)
{
	size_t size;

	size = 0;
	while (*args)
	{
		size++;
		args++;
	}
	return (size);
}

void	builtin_exit(char **args)
{
	int	status;

	if (!args)
	{
		printf("exit\n");
		exit(0);
	}
	if (sizeof_args(args) > 1)
	{
		pop_error("Minishell: exit: too many arguments\n");
		g_shell.exit_status = 1;
	}
	else
	{
		if (check_exit_value(args[0]))
		{
			printf("exit\n");
			exit(ft_atoi(args[0]));
		}
		printf("exit\n");

	}
}
