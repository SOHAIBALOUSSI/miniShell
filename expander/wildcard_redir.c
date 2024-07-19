/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_redir.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sait-alo <sait-alo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 19:46:50 by msaadidi          #+#    #+#             */
/*   Updated: 2024/07/18 13:21:45 by sait-alo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	matching_file(struct dirent *entry, char *filename,
	char **matched, int *count)
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
	entry = readdir(dir);
	while (entry)
	{
		if (entry->d_name[0] == '.')
		{
			entry = readdir(dir);
			continue ;
		}
		if (is_match(filename, entry->d_name))
			matching_file(entry, filename, &matched, count);
		entry = readdir(dir);
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
				break ;
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
