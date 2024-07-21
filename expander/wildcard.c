/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 11:43:35 by sait-alo          #+#    #+#             */
/*   Updated: 2024/07/19 18:05:21 by msaadidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_match(char *pattern, char *str)
{
	if (*pattern == '\0' && *str == '\0')
		return (1);
	if (*pattern == '*')
		return (is_match(pattern + 1, str)
			|| (*str != '\0' && is_match(pattern, str + 1)));
	if (*pattern == *str)
		return (is_match(pattern + 1, str + 1));
	return (0);
}

void	add_file_to_matched(char ***matched, int *count, struct dirent *entry)
{
	(*matched) = m_realloc((*matched),
			sizeof(char *) * ((*count) + 1), sizeof(char *) * ((*count) + 2));
	(*matched)[(*count)] = ft_strdup(entry->d_name);
	(*count)++;
	(*matched)[(*count)] = NULL;
}

char	**get_matching_files(char *pattern)
{
	char			**matched;
	int				count;
	DIR				*dir;
	struct dirent	*entry;

	if (init_vars(&matched, &count, &dir))
		return (NULL);
	entry = readdir(dir);
	while (entry)
	{
		if (entry->d_name[0] == '.')
		{
			entry = readdir(dir);
			continue ;
		}
		if (is_match(pattern, entry->d_name))
			add_file_to_matched(&matched, &count, entry);
		entry = readdir(dir);
	}
	if (count == 0)
	{
		matched = m_alloc(sizeof(char *) * 1, ALLOC);
		matched[0] = NULL;
	}
	return (closedir(dir), matched);
}

void	add_matched_files(char ***matched, char ***new_argv, int *new_argc)
{
	int	j;

	j = -1;
	while ((*matched)[++j])
		realloc_argv(new_argv, new_argc, (*matched)[j]);
	free_strs((*matched));
}

void	expand_wildard(char ***old_argv)
{
	int		i;
	char	**matched;
	char	**new_argv;
	int		new_argc;

	init_expand_vars(&i, &new_argc, &new_argv, &matched);
	while (*old_argv && (*old_argv)[++i])
	{
		if (ft_strchr((*old_argv)[i], '*') != NULL)
		{
			matched = get_matching_files((*old_argv)[i]);
			if (matched && matched[0])
				add_matched_files(&matched, &new_argv, &new_argc);
			else
				realloc_argv(&new_argv, &new_argc, (*old_argv)[i]);
		}
		else
			realloc_argv(&new_argv, &new_argc, (*old_argv)[i]);
	}
	if (!*old_argv)
		return ;
	(*old_argv) = new_argv;
}
