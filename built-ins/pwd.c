
#include "../minishell.h"

int    builtin_pwd(void)
{
	char	pwd[PATH_MAX];

	if (!getcwd(pwd, PATH_MAX))
		return (perror("Minishell: pwd: getcwd"), EXIT_FAILURE);
	printf("%s\n", pwd);
	return (EXIT_SUCCESS);
}
