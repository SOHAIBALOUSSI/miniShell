#include "../minishell.h"

bool	check_n_option(char *arg)
{
	int i;

	if (arg[0] == '-')
	{
		i = 1;
		while (arg[i] == 'n')
			i++;
		if (arg[i] == '\0')
			return (true);
	}
	return (false);
}

int	builtin_echo(char **args)
{
	int i;
	int flag;
	int	ignore_n;

	i = 0;
	flag = 0;
	ignore_n = 0;
	if (!args || !*args) 
		return (write(1, "\n", 1), EXIT_SUCCESS);
	while (args[i])
	{
		if (!ignore_n && check_n_option(args[i]) == true)
		{
			flag = 1;
			i++;
			continue ;
		}
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		ignore_n = 1; // if a the arg is a word , any other -n will be ignored
		i++;
	}
	if (!flag)
		printf("\n");
}
