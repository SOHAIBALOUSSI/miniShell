/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sait-alo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 19:42:23 by sait-alo          #+#    #+#             */
/*   Updated: 2023/11/30 19:42:25 by sait-alo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "../../minishell.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*node;

	node = m_alloc(sizeof(t_list), ALLOC);
	if (!node)
		return (NULL);
	node->content = content;
	node->next = NULL;
	return (node);
}
