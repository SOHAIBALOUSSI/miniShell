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

/// @brief Allocate SIZE bytes of memory and save its address
/// @param __size Size of memory to be allocated
/// @param todo If set to 'FREE,
/// all the Memory allocated during execution will be freed
/// @return The allocated memory block
void	*m_alloc(size_t __size, char todo)
{
	static t_gc	*arena;
	void		*ptr;
	t_gc		*del;

	if (todo == FREE)
	{
		while (arena)
		{
			del = arena;
			arena = arena->next;
			free(del->ptr);
			free(del);
		}
		arena = NULL;
		return (NULL);
	}
	ptr = malloc(__size);
	if (!ptr || m_add_back(&arena, m_new_node(ptr)))
		return (m_alloc(0, FREE));
	return (ptr);
}
