#include "minishell.h"

void search_and_change(t_env **env_lst, char *key, char *new_value)
{
    t_env *tmp;

    tmp = *env_lst;
    while (tmp->next)
    {
        if (!ft_strncmp(key, tmp->key, ft_strlen(key)))
        {
            free(tmp->value);
			printf("changed!\n");
            tmp->value = new_value;
            return ;
        }
        tmp = tmp->next;
    }
    pop_error("Env variable not found\n");
}
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