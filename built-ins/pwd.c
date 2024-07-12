
#include "../minishell.h"

int    builtin_pwd(void)
{
	char	pwd[PATH_MAX];

	if (!getcwd(pwd, PATH_MAX))
		return (printf("%s\n", mshell()->pwd), EXIT_SUCCESS);
	mshell()->pwd = ft_strdup(pwd);
	printf("%s\n", pwd);
	return (EXIT_SUCCESS);
}
