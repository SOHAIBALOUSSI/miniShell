#include "../minishell.h"

void    builtin_cd(char **args)
{
	t_env *home;
	char cwd[PATH_MAX];
	int size;
	char *new_wd;
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
	}
	char *a;
	if (args[0])
	{
		if (args[1])
		{
			pop_error("Minishell: cd: too many arguments\n");
			return ;
		}
		if (ft_strcmp(args[0], "..") == 0)
		{
			getcwd(cwd, PATH_MAX);
			size = ft_strlen(ft_strrchr(cwd, '/'));
			new_wd = ft_substr(cwd, 0, ft_strlen(cwd) - size);
			chdir(new_wd);
		}
		if (ft_strcmp(args[0], "~") == 0)
		{
			if (!home)
				pop_error("Minishell: cd: HOME not set\n");

		}
	}
	else
		printf("bye\n");
}