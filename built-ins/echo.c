#include "../minishell.h"

void	builtin_echo(char **args)
{
	int i;
	int j;
	int flag;
	
	i = 0;
	j = 0;
	flag = 0;
	if (!args || !*args)
	{
		ft_putchar('\n');
		return ;
	}
	while (args[i])
	{
		j = 0;

		if (args[i][0] == '-' && args[i][1] == 'n')
		{
			j = 1;
			while (args[i][j] && args[i][j] == 'n')
				j++;
			flag = 1;
			i++;
			continue;
		}
		else 
			printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (!flag)
		printf("\n");
}
