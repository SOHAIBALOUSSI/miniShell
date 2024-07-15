/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Justice.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sait-alo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 16:29:30 by sait-alo          #+#    #+#             */
/*   Updated: 2024/05/11 16:29:34 by sait-alo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_gc	*m_new_node(void *ptr)
{
	t_gc	*to_collect;

	to_collect = malloc(sizeof(t_gc));
	if (!to_collect)
		return (NULL);
	to_collect->ptr = ptr;
	to_collect->next = NULL;
	return (to_collect);
}

void	m_free(void *ptr)
{
	t_gc	*tmp;
	t_gc	*prev;

	tmp = mshell()->arena;
	prev = NULL;
	while (tmp)
	{
		if (tmp->ptr == ptr)
		{
			if (prev)
				prev->next = tmp->next;
			else
				mshell()->arena = tmp->next;
			free(tmp->ptr);
			free(tmp);
			break ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

int	m_add_back(t_gc **lst, t_gc *new)
{
	static t_gc	*last;

	if (!lst || !new)
		return (EXIT_FAILURE);
	if (*lst == NULL)
	{
		*lst = new;
		last = new;
		return (EXIT_SUCCESS);
	}
	last->next = new;
	last = new;
	return (EXIT_SUCCESS);
}
void	free_arena(void)
{
	t_gc	**arena;
	t_gc	*tmp;

	arena = &(mshell()->arena);
	while (*arena)
	{
		tmp = *arena;
		*arena = (*arena)->next;
		free(tmp->ptr);
		free(tmp);
	}
	*arena = NULL;
}
void	*m_alloc(size_t __size, char todo)
{
	void		*ptr;
	t_gc		**arena;

	arena = &(mshell()->arena);
	if (todo == FREE)
	{
		free_arena();
		return (NULL);
	}
	ptr = malloc(__size);
	if (!ptr || m_add_back(arena, m_new_node(ptr)))
	{
		if (ptr)
			free(ptr);
		free_arena();
		write(2, "Memory allocation failed\n", 26);
		exit(EXIT_FAILURE);
	}
	return (ptr);
}

