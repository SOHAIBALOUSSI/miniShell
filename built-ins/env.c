
#include "../minishell.h"

void	builtin_env(void)
{
	t_env	*tmp;

	tmp = *mshell()->env_list;
	while (tmp)
	{
		if (tmp->value)
			printf("%s=%s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
}

char **get_current_env_array(void)
{
    t_env *env_list;
    t_env *temp;
    int count;
    char **env_array;
	
	env_list = *mshell()->env_list;
	temp = env_list;
	count = 0;
    while (temp)
    {
        count++;
        temp = temp->next;
    }
    env_array = m_alloc(sizeof(char *) * (count + 1), ALLOC);
    temp = env_list;
    count = 0;
    while (temp)
    {
        if (!temp->value)
        {
            temp = temp->next;
            continue;
        }
        env_array[count] = ft_strjoin(temp->key, "=");
        env_array[count] = ft_strjoin(env_array[count], temp->value);
        temp = temp->next;
        count++;
    }
    env_array[count] = NULL;
    return env_array;
}
