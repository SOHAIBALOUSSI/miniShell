#include "../minishell.h"

size_t  sizeof_args(char **args)
{
    size_t size;

    size = 0;
    while (*args)
    {
        size++;
        args++;
    }
    return (size);
}

void    builtin_exit(char **args)
{
    if (!args)
        exit(0);
    if (sizeof_args(args) > 1)
        pop_error("Minishell: exit: too many arguments\n");
    else
    {
        printf("exit\n");
        exit(ft_atoi(args[0]));
    }
}
