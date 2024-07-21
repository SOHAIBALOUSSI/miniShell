/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 15:25:23 by sait-alo          #+#    #+#             */
/*   Updated: 2024/07/19 17:08:01 by msaadidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	append_env(t_env **lst, t_env *new_env)
{
	t_env	*tail;

	if (lst == NULL || new_env == NULL)
		return ;
	new_env->next = NULL;
	if (*lst == NULL)
	{
		*lst = new_env;
		return ;
	}
	tail = *lst;
	while (tail->next)
		tail = tail->next;
	tail->next = new_env;
}

void	set_shlvl(t_env *node)
{
	int		old_lvl;
	t_env	*shlvl;

	if (node)
	{
		old_lvl = ft_atoi(node->value);
		m_free(node->value);
		if (old_lvl < 0)
			node->value = ft_itoa(0);
		else if (old_lvl >= 999)
		{
			pop_error(_SHLVL);
			node->value = ft_itoa(1);
		}
		else
			node->value = ft_itoa(old_lvl + 1);
	}
	else
	{
		shlvl = create_env("SHLVL=1");
		append_env(mshell()->env_list, shlvl);
	}
}

t_env	*setup_clean_env(void)
{
	t_env	*clean_env;
	char	pwd[PATH_MAX];

	clean_env = NULL;
	getcwd(pwd, PATH_MAX);
	append_env(&clean_env, create_env(ft_strjoin("PWD=", pwd)));
	append_env(&clean_env, create_env("SHLVL=0"));
	append_env(&clean_env, create_env("_=/usr/bin/env"));
	return (clean_env);
}

void	get_env_list(char **env)
{
	int		i;
	t_env	*shlvl;
	t_env	**env_list;

	i = -1;
	env_list = mshell()->env_list;
	if (!*env)
		*env_list = setup_clean_env();
	while (env[++i])
		append_env(env_list, create_env(env[i]));
	shlvl = find_env_var("SHLVL", *env_list);
	set_shlvl(shlvl);
}
