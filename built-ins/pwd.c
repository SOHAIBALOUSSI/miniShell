
#include "../minishell.h"

int    builtin_pwd(void)
{
	char	pwd[PATH_MAX];

	if (!getcwd(pwd, PATH_MAX))
		return (printf("%s\n", pwd), EXIT_SUCCESS);
	mshell()->pwd = pwd;
	printf("%s\n", pwd);
	return (EXIT_SUCCESS);
}
