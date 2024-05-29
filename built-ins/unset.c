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
    // search for the key in the env_list
    // delete it
    t_env   *tmp;
    t_env   *del;

    tmp = g_shell.env_list;
    if (!args)
        return ;
    while (*args)
    {
        if (ft_strcmp(tmp->key, *args) == 0)
        {
            del = tmp;
            tmp = tmp->next;
            del_env(del);
            break;
        }
        while (tmp)
        {

            if (tmp->next && ft_strcmp(tmp->next->key, *args) == 0)
            {
                if (!tmp->next->next)
                {
                    del = tmp->next;
                    tmp->next = NULL;
                    del_env(del);
                     break;
                }
                else
                {
                    del = tmp->next;
                    tmp = tmp->next->next;
                    del_env(del);
                    break;
                }
            }
            else
                tmp = tmp->next;
        }
        args++;
    }
}