#include "minishell.h"

t_env   *search_and_change(t_env **env_lst, char *key, )

void	append_env(t_env **lst, t_env *new)
{
	t_env	*last;

	if (lst == NULL)
		return ;
	last = *lst;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	while (last->next)
		last = last->next;
	last->next = new;
}

char *get_key(char *env)
{
	int i = 0;
	char *key;
	if (!env)
		return NULL;
	while (env[i] != '=')
		i++;
	key = ft_substr(env, 0, i);
	return (key);
}

t_env	*create_env(char *env)
{
	t_env	*node;

	node = m_alloc(sizeof(t_env), ALLOC);
	node->key = get_key(env);
	node->value = ft_substr((env + ft_strlen(node->key) + 1), 0, ft_strlen(env));
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