/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 19:48:27 by msaadidi          #+#    #+#             */
/*   Updated: 2024/07/17 17:35:30 by msaadidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	init_vars(char ***matched, int *count, DIR **dir)
{
	(*matched) = NULL;
	(*count) = 0;
	(*dir) = opendir(".");
	if (!(*dir))
		return (1);
	return (0);
}

int	init_vars2(char **matched, int *count, DIR **dir)
{
	(*matched) = NULL;
	(*count) = 0;
	(*dir) = opendir(".");
	if (!(*dir))
	{
		perror("opendir");
		return (1);
	}
	return (0);
}

void	free_strs(char **strs)
{
	int	i;

	i = 0;
	while (strs[i])
	{
		m_free(strs[i]);
		i++;
	}
	m_free(strs);
}

void	realloc_argv(char ***new_argv, int *argc, char *content)
{
	(*new_argv) = m_realloc((*new_argv),
			sizeof(char *) * (*argc), sizeof(char *) * ((*argc) + 2));
	(*new_argv)[(*argc)] = ft_strdup(content);
	(*new_argv)[(*argc) + 1] = NULL;
	(*argc)++;
}

void	init_expand_vars(int *i, int *new_argc,
	char ***new_argv, char ***matched)
{
	(*i) = -1;
	(*new_argc) = 0;
	(*new_argv) = NULL;
	(*matched) = NULL;
}
