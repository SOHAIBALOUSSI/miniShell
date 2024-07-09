#include "../minishell.h"

void	built_ins_err(char *err_key)
{
	// ft_putstr_fd(RED, 2);
	ft_putstr_fd("Minishell: export: `", 2);
	ft_putstr_fd(err_key, 2);
	ft_putstr_fd("\': not a valid identifier\n", 2);
	// ft_putstr_fd(RESTORE, 2);
}
size_t	lst_size(t_env **lst)
{
	t_env	*tmp;
	size_t	size;

	tmp = *lst;
	size = 0;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, "_") == 0)
		{
			tmp = tmp->next;
			continue;
		}
		size++;
		tmp = tmp->next;
	}
	return (size);
}

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

char *get_value(char *key)
{
	t_env *found;

	found = find_env_var(key, *mshell()->env_list);
	if (found)
		return (found->value);
	return (NULL);
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
			{
				mshell()->is_add = 1;
				return (true);
			}
			return (false);
		}
		if (!ft_isalnum(s[i]) && s[i] != '_' )
			return (false);
		i++;
	}
	return (true);
}