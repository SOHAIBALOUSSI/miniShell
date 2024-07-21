/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 16:08:52 by sait-alo          #+#    #+#             */
/*   Updated: 2024/07/19 17:08:01 by msaadidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strndup(char *s1, size_t n)
{
	size_t	i;
	char	*new;

	i = 0;
	if (!s1)
		return (NULL);
	new = m_alloc(sizeof(char) * (n + 1), ALLOC);
	while (s1[i] && i < n)
	{
		new[i] = s1[i];
		i++;
	}
	new[i] = '\0';
	return (new);
}

void	add_arg_to_cmd(t_tree *cmd, char *location, size_t length)
{
	size_t	argc;

	argc = cmd->argc;
	cmd->argv = m_realloc(cmd->argv, sizeof(char *) * (argc + 1),
			sizeof(char *) * (argc + 2));
	cmd->argv[argc] = ft_strndup(location, length);
	cmd->argv[argc + 1] = NULL;
	cmd->argc++;
}

void	add_cmd_to_pipeline(t_tree *pipe, t_tree *cmd)
{
	size_t	cmdc;

	cmdc = pipe->pipe_count;
	pipe->pipe_line = m_realloc(pipe->pipe_line, sizeof(t_tree *) * (cmdc + 1),
			sizeof(t_tree *) * (cmdc + 2));
	pipe->pipe_line[cmdc] = cmd;
	pipe->pipe_line[cmdc + 1] = NULL;
	pipe->pipe_count++;
}
