/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 15:32:22 by sait-alo          #+#    #+#             */
/*   Updated: 2024/07/19 17:08:01 by msaadidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	check_n_option(char *arg)
{
	int	i;

	if (arg[0] == '-' && arg[1])
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
	int	i;
	int	flag;
	int	ignore_n;

	i = -1;
	flag = 0;
	ignore_n = 0;
	if (!*args)
		return (write(1, "\n", 1), EXIT_SUCCESS);
	while (args[++i])
	{
		if (!ignore_n && check_n_option(args[i]) == true)
		{
			flag = 1;
			continue ;
		}
		write(1, args[i], ft_strlen(args[i]));
		if (args[i + 1])
			write(1, " ", 1);
		ignore_n = 1;
	}
	if (!flag)
		write(1, "\n", 1);
	return (0);
}
