/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 19:37:31 by msaadidi          #+#    #+#             */
/*   Updated: 2024/07/16 19:59:13 by msaadidi         ###   ########.fr       */
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

int	init_vars(char ***matched, int *count, DIR **dir)
{
	*matched = NULL;
	*count = 0;
	(*dir) = opendir(".");
	if (!(*dir))
	{
		perror("opendir");
		return (1);
	}
	return (0);
}

int	init_vars2(char **matched, int *count, DIR **dir)
{
	*matched = NULL;
	*count = 0;
	*dir = opendir(".");
	if (!(*dir))
	{
		perror("opendir");
		return (1);
	}
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
	(*new_argv) = m_realloc((*new_argv), sizeof(char *) * (*argc), sizeof(char *) * ((*argc) + 2));
	(*new_argv)[(*argc)] = ft_strdup(content);
	(*new_argv)[(*argc) + 1] = NULL;
	(*argc)++;
}

void	add_matched_files(char ***matched, char ***new_argv, int *new_argc)
{
	int	j;

	j = -1;
	while ((*matched)[++j])
		realloc_argv(new_argv, new_argc, (*matched)[j]);
	free_strs((*matched));
}

void	init_expand_vars(int *i, int *new_argc, char ***new_argv, char ***matched)
{
	(*i) = -1;
	(*new_argc) = 0;
	(*new_argv) = NULL;
	(*matched) = NULL;
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

void	matching_file(struct dirent *entry, char *filename, char **matched, int *count)
{
	if (is_match(filename, entry->d_name))
	{
		if (*count == 0)
			(*matched) = ft_strdup(entry->d_name);
		else
		{
			if ((*matched))
				m_free((*matched));
		}
		(*count)++;
	}
}

char	*get_matching_file(char *filename, int *count)
{
	char			*matched;
	DIR				*dir;
	struct dirent	*entry;

	if (init_vars2(&matched, count, &dir))
		return (NULL);
	while ((entry = readdir(dir)) != NULL)
	{
		if (entry->d_name[0] == '.')
			continue;
		if (is_match(filename, entry->d_name))
			matching_file(entry, filename, &matched, count);
	}
	closedir(dir);
	if (*count == 0)
		return (NULL);
	return (matched);
}

void	expand_redir_wildcard(t_redir **redir)
{
	t_redir	*current;
	char	*matched;
	int		count;

	current = (*redir);
	while (current)
	{
		if (ft_strchr(current->file_name, '*') != NULL)
		{
			matched = get_matching_file(current->file_name, &count);
			if (count > 1)
			{
				current->is_ambiguous = 1;
				break;
			}
			else if (matched)
			{
				if (current->file_name)
					m_free(current->file_name);
				current->file_name = matched;
			}
		}
		current = current->next;
	}
}
