/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 15:32:22 by sait-alo          #+#    #+#             */
/*   Updated: 2024/07/19 17:08:01 by msaadidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	syntax_err(t_token *current)
{
	char	*err;

	if (current == NULL)
		err = ft_strdup("newline");
	else
		err = ft_substr(current->location.location,
				0, current->location.length);
	ft_putstr_fd("Minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd(err, 2);
	ft_putstr_fd("\'\n", 2);
	m_free(err);
}

char	*remove_quotes(char *str)
{
	int				i;
	int				j;
	t_quote_state	qs;
	char			*new_str;

	i = 0;
	j = 0;
	qs = (t_quote_state){0};
	new_str = m_alloc(ft_strlen(str) + 1 * sizeof(char), ALLOC);
	mshell()->expand_oho = 1;
	while (str[i])
	{
		if (str[i] == '\'' && !qs.in_dquote)
			qs.in_squote = !qs.in_squote;
		else if (str[i] == '\"' && !qs.in_squote)
			qs.in_dquote = !qs.in_dquote;
		else
			new_str[j++] = str[i];
		i++;
	}
	new_str[j] = '\0';
	m_free(str);
	return (new_str);
}

bool	has_quotes(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == SQUOTE || str[i] == DQUOTE)
			return (true);
	}
	return (false);
}

int	check_quotes_and_parens(void)
{
	if ((mshell()->single_quote_count % 2) != 0 || \
			(mshell()->double_quote_count % 2) != 0)
	{
		pop_error("Minishell: syntax error 'unclosed quotes'\n");
		return (0);
	}
	else if (mshell()->closed_paren_count != mshell()->open_paren_count)
	{
		pop_error("Minishell: syntax error 'unclosed parenthesis'\n");
		return (0);
	}
	return (1);
}

int	check_heredoc(t_token *current)
{
	if (current->type == _HEREDOC)
	{
		if (!current->next || !is_word(current->next->type))
		{
			syntax_err(current->next);
			return (0);
		}
		current->delimiter = ft_strndup(current->next->location.location,
				current->next->location.length);
		if (has_quotes(current->delimiter))
			current->delimiter = remove_quotes(current->delimiter);
		current->heredoc_file = read_heredoc(current->delimiter);
	}
	return (1);
}
