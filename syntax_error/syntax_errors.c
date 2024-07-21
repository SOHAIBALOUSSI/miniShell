/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_errors.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 15:32:22 by sait-alo          #+#    #+#             */
/*   Updated: 2024/07/19 17:08:01 by msaadidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	check_pipe_and(t_token *current)
{
	if (is_pipe_or_and(current->type))
	{
		if ((!current->prev || !current->next)
			|| (is_pipe_or_and(current->prev->type)))
		{
			syntax_err(current);
			return (0);
		}
	}
	return (1);
}

static int	check_redirection(t_token *current)
{
	if (is_redirection(current->type))
	{
		if ((!current->next || !is_word(current->next->type)))
		{
			syntax_err(current->next);
			return (0);
		}
	}
	return (1);
}

static int	check_parentheses(t_token *current)
{
	if (current->type == _PAREN_OPEN && current->next)
	{
		if (is_pipe_or_and(current->next->type)
			|| current->next->type == _PAREN_CLOSED)
		{
			syntax_err(current->next);
			return (0);
		}
	}
	else if (current->type == _PAREN_CLOSED && current->next)
	{
		if (current->next->type == _PAREN_OPEN
			|| current->next->type == _WORD)
		{
			syntax_err(current->next);
			return (0);
		}
	}
	return (1);
}

static int	check_word(t_token *current)
{
	if (is_word(current->type) && current->next)
	{
		if (current->next->type == _PAREN_OPEN)
		{
			if (!current->prev || !is_word(current->prev->type))
				syntax_err(current->next->next);
			else
				syntax_err(current->next);
			return (0);
		}
	}
	return (1);
}

int	catch_syntax_errors(t_token *token_lst)
{
	t_token	*current;

	current = token_lst;
	while (current)
	{
		if (current->type == _SPACE)
		{
			current = current->next;
			continue ;
		}
		if (!check_pipe_and(current) || !check_redirection(current)
			|| !check_parentheses(current) || !check_word(current)
			|| !check_heredoc(current))
		{
			mshell()->exit_status = 2;
			return (EXIT_FAILURE);
		}
		current = current->next;
	}
	if (!check_quotes_and_parens())
	{
		mshell()->exit_status = 2;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
