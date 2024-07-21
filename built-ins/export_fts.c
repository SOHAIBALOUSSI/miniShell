/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_fts.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 15:32:22 by sait-alo          #+#    #+#             */
/*   Updated: 2024/07/19 17:08:01 by msaadidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env	*find_env_var(char *key, t_env *env_list)
{
	t_env	*tmp;

	tmp = env_list;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

char	*get_value(char *key)
{
	t_env	*found;

	found = find_env_var(key, *mshell()->env_list);
	if (found)
		return (found->value);
	return (NULL);
}

bool	has_add_sign(char *s)
{
	int	i;

	i = 0;
	while (s[i] && s[i] != '=')
	{
		if (s[i] == '+')
			return (true);
		i++;
	}
	return (false);
}

bool	has_only_equal_sign(char *s)
{
	int	i;

	i = 0;
	while (s[i] && s[i + 1])
	{
		if (s[i] != '+' && s[i + 1] == '=')
			return (true);
		i++;
	}
	return (false);
}

bool	is_valid_key(char *s)
{
	int	i;

	i = 0;
	if (!ft_isalpha(s[i]) && s[i] != '_')
		return (false);
	while (s[i] && s[i] != '=')
	{
		if (s[i] == '+')
		{
			if (s[i + 1] == '=')
				return (true);
			return (false);
		}
		if (!ft_isalnum(s[i]) && s[i] != '_' )
			return (false);
		i++;
	}
	return (true);
}
