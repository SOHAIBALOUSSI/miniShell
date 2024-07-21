/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 15:32:22 by sait-alo          #+#    #+#             */
/*   Updated: 2024/07/19 17:08:01 by msaadidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	del_env(t_env *env)
{
	if (!env)
		return ;
	m_free(env->key);
	m_free(env->value);
	m_free(env);
}

void	del_head_env(void)
{
	t_env	*tmp;

	if (!*mshell()->env_list)
		return ;
	tmp = *mshell()->env_list;
	*mshell()->env_list = (*mshell()->env_list)->next;
	del_env(tmp);
}

int	builtin_unset(char **args)
{
	t_env	*tmp;
	t_env	*del;

	while (*args)
	{
		tmp = *mshell()->env_list;
		if (tmp && ft_strcmp(tmp->key, *args) == 0)
		{
			del_head_env();
			continue ;
		}
		while (tmp && tmp->next)
		{
			if (ft_strcmp(tmp->next->key, *args) == 0)
			{
				del = tmp->next;
				tmp->next = tmp->next->next;
				del_env(del);
				break ;
			}
			tmp = tmp->next;
		}
		args++;
	}
	return (EXIT_SUCCESS);
}
