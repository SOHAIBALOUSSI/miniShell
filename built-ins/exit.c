#include "../minishell.h"
#include <stdbool.h>
#include <stdio.h>


bool	check_exit_value(char *value)
{
	int			i;
	int			s;

	i = 0;
	while (value[i] == ' ' && value[i] >= '\t' && value[i] <= '\r')
		i++;
	if (value[i] == '+' || value[i] == '-')
		i++;
	while (value[i])
	{
		if (!(value[i] >= '0' && value[i] <= '9'))
			return (false);
		i++;
	}
	return (true);
}

void 	exit_clean(int exit_status)
{
	// printf("exit\n");
	// m_alloc(0, FREE);
	exit(exit_status);
}
void	builtin_exit(char **args)
{
	int	status;

	if (!args || !args[0])
	{
		printf("exit\n");
		exit_clean(g_shell.exit_status);
	}
	else if (args[0] && args[1])
		pop_error("Minishell: exit: too many arguments\n");
	else if (check_exit_value(args[0]))
	{
		printf("exit\n");
		exit_clean(ft_atoi(args[0]));
	}
	else
	{
		printf("exit\n");
		ft_putstr_fd("Minishell: exit: ", 2);
		ft_putstr_fd(args[0], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		exit_clean(2);
	}
}
