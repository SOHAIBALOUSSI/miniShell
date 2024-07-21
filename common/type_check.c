/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type_check.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 15:31:58 by sait-alo          #+#    #+#             */
/*   Updated: 2024/07/19 17:08:01 by msaadidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r');
}

int	is_op(char c, char c1)
{
	if (c == '&' && c1 == '&')
		return (1);
	return (c == '|' || c == '>' || c == '<' || c == '('
		|| c == ')' || c == '*' || c == '$');
}

int	is_redirection(t_tok type)
{
	return (type == _APPEND || type == _HEREDOC
		|| type == _RED_IN || type == _RED_OUT);
}

int	is_pipe_or_and(t_tok type)
{
	return (type == _PIPE || type == _OR || type == _AND);
}

int	is_word(t_tok type)
{
	return (type == _WORD || type == _ENV
		|| type == _WILDCARD || type == _QUOTE);
}
