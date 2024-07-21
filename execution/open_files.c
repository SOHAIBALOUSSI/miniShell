/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: m3ayz00 <m3ayz00@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 18:08:33 by msaadidi          #+#    #+#             */
/*   Updated: 2024/07/20 23:18:54 by m3ayz00          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_input_redirection(t_redir *current, int *errors, int is_builtin)
{
	if (current->is_ambiguous)
	{
		(*errors)++;
		return (perror_ambiguous(current->file_name));
	}
	current->fds[0] = open(current->file_name, O_RDONLY);
	if (current->fds[0] < 0)
	{
		(*errors)++;
		return (perror_file(current->file_name), 1);
	}
	current->original_in = dup(STDIN_FILENO);
	dupping(is_builtin, current, 0, STDIN_FILENO);
	close(current->fds[0]);
	return (0);
}

int	handle_output_redirection(t_redir *current, int *errors, int is_builtin)
{
	int	flags;

	if (current->is_ambiguous)
	{
		(*errors)++;
		return (perror_ambiguous(current->file_name));
	}
	flags = O_WRONLY | O_CREAT;
	if (current->type == _RED_OUT)
		flags |= O_TRUNC;
	else
		flags |= O_APPEND;
	current->fds[1] = open(current->file_name, flags, 0644);
	if (current->fds[1] < 0)
	{
		perror_file(current->file_name);
		(*errors)++;
		return (1);
	}
	current->original_out = dup(STDOUT_FILENO);
	dupping(is_builtin, current, 1, STDOUT_FILENO);
	close(current->fds[1]);
	return (0);
}

int	handle_heredoc(t_redir *current, int *errors)
{
	current->fds[0] = open(current->file_name, O_RDONLY);
	if (current->fds[0] < 0)
	{
		(*errors)++;
		return (perror_file(current->file_name), 1);
	}
	current->original_in = dup(STDIN_FILENO);
	dup2(current->fds[0], STDIN_FILENO);
	close(current->fds[0]);
	return (0);
}

void	handle_redirections(t_redir *redir_list, int *ext, int is_builtin)
{
	t_redir	*current;
	int		errors;

	errors = 0;
	current = redir_list;
	*ext = EXIT_SUCCESS;
	while (current)
	{
		if ((current->type == _RED_IN)
			&& (handle_input_redirection(current, &errors, is_builtin)))
			break ;
		else if ((current->type == _RED_OUT || current->type == _APPEND)
			&& (handle_output_redirection(current, &errors, is_builtin)))
			break ;
		else if ((current->type == _HEREDOC)
			&& (handle_heredoc(current, &errors)))
			break ;
		current = current->next;
	}
	if (errors > 0)
		*ext = EXIT_FAILURE;
	if (!is_builtin && (*ext) == EXIT_FAILURE)
		exit((*ext));
}

void	restore_redirections(t_redir *redir)
{
	t_redir	*current;

	current = redir;
	while (current)
	{
		if (current->original_in != -1)
		{
			dup2(current->original_in, STDIN_FILENO);
			close(current->original_in);
		}
		if (current->original_out != -1)
		{
			dup2(current->original_out, STDOUT_FILENO);
			close(current->original_out);
		}
		current = current->next;
	}
}
