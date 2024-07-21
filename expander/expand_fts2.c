/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_fts2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 15:32:22 by sait-alo          #+#    #+#             */
/*   Updated: 2024/07/20 15:18:00 by msaadidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	**add_to_argv(char *expanded_arg, char ***expanded_argv)
{
	int		i;
	char	**new_argv;

	i = 0;
	while ((*expanded_argv)[i])
		i++;
	new_argv = m_realloc(*expanded_argv, \
					sizeof(char *) * (i + 1), \
					sizeof(char *) * (i + 2));
	new_argv[i] = expanded_arg;
	new_argv[i + 1] = NULL;
	return (new_argv);
}

static void	split_and_add_to_new_argv(char *expanded_arg, char ***expanded_argv)
{
	int		i;
	char	**split;

	split = ft_split(expanded_arg, " \n\t\v\t\r");
	i = 0;
	while (split[i])
	{
		add_to_new_argv(split[i], expanded_argv, false);
		i++;
	}
}

void	add_to_new_argv(char *expanded_arg,
		char ***expanded_argv, bool to_split)
{
	if (to_split == true)
		split_and_add_to_new_argv(expanded_arg, expanded_argv);
	else
		*expanded_argv = add_to_argv(expanded_arg, expanded_argv);
}

int	check_special_chars(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
			return (1);
		i++;
	}
	return (0);
}

int	dollar_count(char *arg)
{
	int	count;
	int	i;

	i = 0;
	count = 0;
	while (arg[i])
	{
		if (arg[i] == '$')
			count++;
		i++;
	}
	return (count);
}
