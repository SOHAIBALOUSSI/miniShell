/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/24 13:30:38 by sait-alo          #+#    #+#             */
/*   Updated: 2024/07/19 17:08:01 by msaadidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "../minishell.h"

static char	*read_line(int fd, char *content, char *buffer)
{
	ssize_t	readed;
	char	*tmp;

	readed = 1;
	while (readed > 0 && !(gnl_strchr(content, '\n')))
	{
		readed = read(fd, buffer, 1);
		if (readed < 0)
		{
			m_free(buffer);
			return (NULL);
		}
		else if (readed == 0)
			break ;
		if (!content)
			content = ft_strdup("");
		buffer[readed] = '\0';
		tmp = content;
		content = ft_strjoin(tmp, buffer);
		m_free(tmp);
		tmp = NULL;
	}
	m_free(buffer);
	return (content);
}

static char	*get_rest(char *line)
{
	ssize_t	i;
	char	*result;

	i = 0;
	while (line[i] != '\n' && line[i] != '\0')
		i++;
	if (line[i] == '\0')
		return (NULL);
	result = ft_substr(line, i + 1, ft_strlen(line) - i);
	if (*result == 0)
	{
		m_free(result);
		result = NULL;
	}
	line[i + 1] = '\0';
	return (result);
}

char	*get_next_line(int fd)
{
	static char	*content;
	char		*buffer;
	char		*line;

	line = NULL;
	buffer = m_alloc((2) * sizeof(char), ALLOC);
	if (fd < 0)
	{
		m_free(buffer);
		buffer = NULL;
		return (NULL);
	}
	if (!buffer)
		return (NULL);
	line = read_line(fd, content, buffer);
	if (!line)
		return (NULL);
	content = get_rest(line);
	return (line);
}
