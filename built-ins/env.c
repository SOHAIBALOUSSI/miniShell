
#include "../minishell.h"

void	builtin_env(void)
{
	t_env	*tmp;

	tmp = mshell()->env_list;
	while (tmp)
	{
		if (ft_strcmp(tmp->value, ""))
			printf("%s=%s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
}