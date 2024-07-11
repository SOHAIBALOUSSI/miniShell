#include "minishell.h"

void append_env(t_env **lst, t_env *new_env)
{
    t_env *tail;

    if (lst == NULL || new_env == NULL)
        return;
    new_env->next = NULL;
    if (*lst == NULL)
    {
        *lst = new_env;
        return;
    }
    tail = *lst;
    while (tail->next)
        tail = tail->next;
    tail->next = new_env;
}

char *get_key(char *s)
{
	int i;

	i = 0;
	while (s[i] && s[i] != '=' && s[i] != '+')
		i++;
	return(ft_substr(s, 0, i));
}

void set_shlvl(t_env *node)
{
	int old_lvl;

	if (node)
	{
		old_lvl = ft_atoi(node->value);
		if (old_lvl < 0)
			node->value = ft_itoa(0);
		else if (old_lvl >= 999)
		{
			pop_error(HIGH_SHLVL);
			node->value = ft_itoa(1);
		}
		else
			node->value = ft_itoa(old_lvl + 1);
	}
	else
		append_env(mshell()->env_list, create_env("SHLVL=1"));
}

char	*_get_value(char *env)
{
	int i;

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
	
	value = NULL;
	node = m_alloc(sizeof(t_env), ALLOC);
	node->key = get_key(env);
	value = _get_value(env);
	node->value = value;
	return (node);
}

t_env	*setup_clean_env(void)
{
	t_env *clean_env;
	char	pwd[PATH_MAX];

	clean_env = NULL;
	getcwd(pwd, PATH_MAX);
	append_env(&clean_env, create_env(ft_strjoin("PWD=", pwd)));
	append_env(&clean_env, create_env("SHLVL=0"));
	append_env(&clean_env, create_env("_=/usr/bin/env"));
	return (clean_env);
}


void get_env_list(char **env)
{
	int i;
    t_env **env_list;

	i = -1;
	env_list = mshell()->env_list;
	if (!*env)
		*env_list = setup_clean_env();
	while (env[++i])
		append_env(env_list, create_env(env[i]));
	set_shlvl(find_env_var("SHLVL", *env_list));
}
