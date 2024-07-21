/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 15:32:22 by sait-alo          #+#    #+#             */
/*   Updated: 2024/07/19 17:08:01 by msaadidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	read_expand_write(char *file_name)
{
	char	*line;
	char	*heredoc;
	int		fd;
	char	*expandheredoc;

	fd = open(file_name, O_RDONLY);
	if (fd < 0)
		return (pop_error("open() failed in read_expand_write\n"), -1);
	heredoc = ft_strdup("");
	line = get_next_line(fd);
	if (!line)
		return (-1);
	while (line != NULL)
	{
		heredoc = ft_strjoin(heredoc, line);
		m_free(line);
		line = get_next_line(fd);
	}
	expandheredoc = expand_heredoc(heredoc);
	close(fd);
	fd = open(file_name, O_WRONLY | O_TRUNC);
	if (fd < 0)
		return (-1);
	return (write(fd, expandheredoc, ft_strlen(expandheredoc)), close(fd), 0);
}

static char	*expand_heredoc_line(char *heredoc_line)
{
	int		i;
	char	*result;

	i = 0;
	result = ft_strdup("");
	while (heredoc_line[i])
	{
		if (heredoc_line[i] == '$' && is_exp(heredoc_line[i + 1]))
			result = handle_dollar_sign(heredoc_line, &i, result);
		else
			result = ft_strjoin_char(result, heredoc_line[i]);
		i++;
	}
	return (result);
}

char	*expand_heredoc(char *heredoc_content)
{
	char	*result;
	char	*line;
	char	**content;
	int		i;

	if (mshell()->expand_oho == 1)
		return (heredoc_content);
	i = 0;
	result = ft_strdup("");
	content = ft_split(heredoc_content, "\n");
	while (content[i])
	{
		line = expand_heredoc_line(content[i]);
		result = ft_strjoin(result, line);
		result = ft_strjoin_char(result, '\n');
		m_free(line);
		i++;
	}
	return (result);
}
