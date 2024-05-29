#include "minishell.h"

void	append_env(t_env **lst, t_env *new_env)
{
	t_env	*tail;

	if (lst == NULL)
		return ;
	tail = *lst;
	if (*lst == NULL)
	{
		*lst = new_env;
		return ;
	}
	while (tail->next)
		tail = tail->next;
	tail->next = new_env;
}

char *get_key(char *s)
{
	int i = 0;

	while (s[i] && s[i] != '=' && s[i] != '+')
		i++;
	return(ft_substr(s, 0, i));
}

t_env	*create_env(char *env)
{
	t_env	*node;
	char	*value;

	node = m_alloc(sizeof(t_env), ALLOC);
	node->key = get_key(env);
	value = ft_strchr(env, '=');
	if (!value)
		node->value = ft_strdup("");
	else
		node->value = ft_strdup(value);
	return (node);
}

t_env	*get_env_list(char **env)
{
	int 	i;
	t_env	*env_list;

	i = 0;
	env_list = NULL;
	while (env[i])
	{
		append_env(&env_list, create_env(env[i]));
		i++;
	}
	return (env_list);
}