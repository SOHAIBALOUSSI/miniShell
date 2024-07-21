/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 16:14:45 by sait-alo          #+#    #+#             */
/*   Updated: 2024/07/19 17:08:01 by msaadidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	pop_error(char *error_msg)
{
	ft_putstr_fd(error_msg, 2);
}

static void	remove_head(t_token **head)
{
	t_token	*tmp;

	tmp = *head;
	*head = (*head)->next;
	if (*head)
		(*head)->prev = NULL;
	m_free(tmp);
}

static void	remove_spaces(t_token **head)
{
	t_token	*current;
	t_token	*next;

	if (!head || !*head)
		return ;
	current = *head;
	while (current && current->type == _SPACE)
	{
		remove_head(head);
		current = *head;
	}
	while (current)
	{
		next = current->next;
		while (next && next->type == _SPACE)
		{
			current->next = next->next;
			if (next->next)
				next->next->prev = current;
			m_free(next);
			next = current->next;
		}
		current = next;
	}
}

static void	string_concatenation(t_token **head)
{
	t_token	*curr;
	t_token	*prev;

	curr = *head;
	prev = NULL;
	while (curr)
	{
		if (is_word(curr->type) && prev && is_word(prev->type))
		{
			prev->location.length += curr->location.length;
			prev->next = curr->next;
			m_free(curr);
			curr = prev;
		}
		prev = curr;
		curr = curr->next;
	}
}

void	refint_tokens(t_token **head)
{
	string_concatenation(head);
	remove_spaces(head);
}
