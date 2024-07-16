/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: m3ayz00 <m3ayz00@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 19:37:31 by msaadidi          #+#    #+#             */
/*   Updated: 2024/07/17 00:00:14 by m3ayz00          ###   ########.fr       */
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
	while ((entry = readdir(dir)) != NULL)
	{
		if (entry->d_name[0] == '.')
			continue;
		if (is_match(pattern, entry->d_name))
			add_file_to_matched(&matched, &count, entry);
	}
	closedir(dir);
	if (count == 0)
	{
		matched = malloc(sizeof(char *) * 1);
		matched[0] = NULL;
	}
	return (matched);
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
	while ((*old_argv)[++i])
	{
		if (ft_strchr((*old_argv)[i], '*') != NULL)
		{
			matched = get_matching_files((*old_argv)[i]);
			if (matched[0])
				add_matched_files(&matched, &new_argv, &new_argc);
			else
			{
				free_strs(matched);
				realloc_argv(&new_argv, &new_argc, (*old_argv)[i]);
			}
		}
		else
			realloc_argv(&new_argv, &new_argc, (*old_argv)[i]);
	}
	free_strs((*old_argv));
	(*old_argv) = new_argv;
}
