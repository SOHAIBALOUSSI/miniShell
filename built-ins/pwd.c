
#include "../minishell.h"
#include <time.h>

void    builtin_pwd(char **args)
{
    (void)args;
    char *pwd;

    pwd = getcwd(NULL, PATH_MAX);
    if (!pwd)
    {
        pop_error("getcwd() failed!\n");
        exit(-1);
    }
    printf("%s\n", pwd);
    free(pwd);
    pwd = NULL;
}
