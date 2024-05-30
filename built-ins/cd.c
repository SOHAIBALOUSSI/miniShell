#include "../minishell.h"

void    builtin_cd(char **args)
{
    t_env *home;

    // cd without args should change dir to $HOME
        // if $HOME dosen't exist pop error
    // cd with - should change dir to $OLDPWD
        // same ..
    // cd  .. should change dir the the prev dir.
    if (!args || !*args)
    {
        home = find_env_var("HOME", g_shell.env_list); 
        if (!home)
            pop_error("Minishell: cd: HOME not set\n");
        else
            chdir(home->value);
        // printf("hi\n");
    }
    if (args[0])
    {
        if (args[1])
        {
            pop_error("Minishell: cd: too many arguments\n");
            return ;
        }
        else if (ft_strcmp(args[0], ".."))
            chdir(find_env_var("OLDPWD", g_shell.env_list)->value);
    }
    else
        printf("bye\n");
}