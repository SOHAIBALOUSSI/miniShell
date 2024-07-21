/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 15:32:22 by sait-alo          #+#    #+#             */
/*   Updated: 2024/07/19 17:08:01 by msaadidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	builtin_env(void)
{
	t_env	*tmp;

	tmp = *mshell()->env_list;
	while (tmp)
	{
		if (tmp->value)
			printf("%s=%s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
	return (0);
}

size_t	_lst_size(t_env *lst)
{
	size_t	count;
	t_env	*tmp;

	tmp = lst;
	count = 0;
	while (tmp)
	{
		if (!tmp->value)
		{
			tmp = tmp->next;
			continue ;
		}
		count++;
		tmp = tmp->next;
	}
	return (count);
}

char	**get_current_env_array(void)
{
	t_env	*env_list;
	t_env	*tmp;
	size_t	count;
	char	**env_array;

	count = 0;
	env_list = *mshell()->env_list;
	env_array = m_alloc(sizeof(char *) * (_lst_size(env_list) + 1), ALLOC);
	tmp = env_list;
	while (tmp)
	{
		if (!tmp->value)
		{
			tmp = tmp->next;
			continue ;
		}
		env_array[count] = ft_strjoin(tmp->key, "=");
		env_array[count] = ft_strjoin(env_array[count], tmp->value);
		tmp = tmp->next;
		count++;
	}
	env_array[count] = NULL;
	return (env_array);
}
