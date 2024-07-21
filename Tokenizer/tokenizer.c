/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 16:14:39 by sait-alo          #+#    #+#             */
/*   Updated: 2024/07/19 17:08:01 by msaadidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_tok	decode_type(char c1, char c2)
{
	t_tok	type;

	type = (c1 == '(') * _PAREN_OPEN + (c1 == ')') * _PAREN_CLOSED
		+ (c1 == '$') * _ENV + (c1 == '*') * _WILDCARD
		+ (c1 == '|' && c2 != '|') * _PIPE
		+ (c1 == '|' && c2 == '|') * _OR
		+ (c1 == '<' && c2 != '<') * _RED_IN
		+ (c1 == '<' && c2 == '<') * _HEREDOC
		+ (c1 == '>' && c2 != '>') * _RED_OUT
		+ (c1 == '>' && c2 == '>') * _APPEND
		+ (c1 == '&' && c2 == '&') * _AND;
	return (type);
}

void	add_spact_token(t_token **head, char **start)
{
	t_token	*token;

	while (is_space(**start))
		(*start)++;
	token = creatt_token(_SPACE, *start, 1);
	append_token(head, token);
}

t_token	*tokenize_line(char *input)
{
	t_token	*head;
	char	*start;

	head = NULL;
	start = input;
	while (*input)
	{
		if (*input && is_space(*input))
			add_spact_token(&head, &input);
		else if (*input && is_op(*input, *(input + 1)))
			input += add_op_token(&head, *input, *(input + 1), input);
		else if (*input && (*input == SQUOTE || *input == DQUOTE))
		{
			start = input;
			input += add_quott_token(&head, input);
		}
		else
		{
			start = input;
			input += add_word_token(&head, start);
		}
	}
	return (refint_tokens(&head), head);
}
