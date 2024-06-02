#include "../minishell.h"

int	builtin_echo(char **args)
{
	int i;
	int j;
	int flag;
	
	i = 0;
	j = 0;
	flag = 0;
	if (!args || !*args) 
		return (write(1, "\n", 1), EXIT_SUCCESS);
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
			continue; // if a char after -nnnnnnx 
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
