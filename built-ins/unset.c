#include "../minishell.h"

void	del_env(t_env *env)
{
	if (!env)
		return ;
	free(env->key);
	free(env->value);
	free(env);
}

void    builtin_unset(char **args)
{
    t_env   *tmp;
    t_env   *del;

    if (!args)
        return ;
    while (*args)
    {
        tmp = g_shell.env_list;
        if (tmp && ft_strcmp(tmp->key, *args) == 0)
        {
            del = tmp;
            g_shell.env_list = tmp->next;
            del_env(del);
        }
        else
        {
            while (tmp && tmp->next)
            {
                if (ft_strcmp(tmp->next->key, *args) == 0)
                {
                    del = tmp->next;
                    tmp->next = tmp->next->next;
                    del_env(del);
                    break;
                }
                tmp = tmp->next;
            }
        }
        args++;
    }
}

