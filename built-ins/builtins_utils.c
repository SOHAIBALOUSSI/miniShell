#include "../minishell.h"

void	exit_clean(int exit_status)
{
	m_alloc(0, FREE);
	exit(exit_status);
}

void	built_ins_err(char *err_key)
{
	ft_putstr_fd("Minishell: export: `", 2);
	ft_putstr_fd(err_key, 2);
	ft_putstr_fd("\': not a valid identifier\n", 2);
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