
#include "../minishell.h"

int    builtin_pwd(void)
{
	char	pwd[PATH_MAX];

	if (!getcwd(pwd, PATH_MAX))
		return (printf("%s\n", mshell()->pwd), EXIT_SUCCESS);
	printf("%s\n", pwd);
	return (EXIT_SUCCESS);
}
