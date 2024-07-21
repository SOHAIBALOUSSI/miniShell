/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 16:29:30 by sait-alo          #+#    #+#             */
/*   Updated: 2024/07/19 17:08:01 by msaadidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	t_gc	**tmp;
	t_gc	*current;
	t_gc	*prev;

	tmp = &(mshell()->arena);
	current = *tmp;
	prev = NULL;
	while (current)
	{
		if (current->ptr == ptr)
		{
			if (prev)
				prev->next = current->next;
			else
				*tmp = current->next;
			if (current->next)
				current->next->prev = prev;
			free(current->ptr);
			current->ptr = NULL;
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

int	m_add_back(t_gc **lst, t_gc *new)
{
	t_gc	*last;

	if (!lst || !new)
		return (EXIT_FAILURE);
	new->next = NULL;
	if (!*lst)
	{
		*lst = new;
		new->prev = NULL;
	}
	else
	{
		last = *lst;
		while (last->next)
			last = last->next;
		last->next = new;
		new->prev = last;
	}
	return (EXIT_SUCCESS);
}

void	free_arena(void)
{
	t_gc	**arena;
	t_gc	*tmp;

	arena = &(mshell()->arena);
	if (!arena || !*arena)
		return ;
	while (*arena)
	{
		tmp = *arena;
		*arena = (*arena)->next;
		free(tmp->ptr);
		tmp->ptr = NULL;
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
	ptr = NULL;
	ptr = malloc(__size);
	if (!ptr || m_add_back(arena, m_new_node(ptr)))
	{
		if (ptr)
			free(ptr);
		free_arena();
		write(2, "Memory allocation failed\n", 26);
		exit(EXIT_FAILURE);
	}
	ft_memset(ptr, 0, __size);
	return (ptr);
}
