/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 16:15:36 by sait-alo          #+#    #+#             */
/*   Updated: 2024/07/19 17:08:01 by msaadidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_env_key(char *s)
{
	int	i;

	i = 0;
	while (s[i] && s[i] != '=' && s[i] != '+')
		i++;
	return (ft_substr(s, 0, i));
}

char	*get_env_value(char *env)
{
	int	i;

	i = 0;
	while (env[i] && env[i] != '=')
		i++;
	if (env[i] == '=')
	{
		if (env[i + 1])
			return (ft_strdup(env + i + 1));
		else
			return (ft_strdup(""));
	}
	return (NULL);
}

t_env	*create_env(char *env)
{
	t_env	*node;
	char	*value;

	node = m_alloc(sizeof(t_env), ALLOC);
	node->key = get_env_key(env);
	value = get_env_value(env);
	node->value = value;
	return (node);
}

void	assign_exit(int status)
{
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
			printf("Quit (core dumped)\n");
		mshell()->exit_status = 128 + WTERMSIG(status);
	}
	else
		mshell()->exit_status = WEXITSTATUS(status);
}
