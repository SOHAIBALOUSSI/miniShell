#include "../minishell.h"

static char	*get_home_dir(void)
{
	t_env	*home;

	home = find_env_var("HOME", g_shell.env_list);
	if (!home)
	{
		pop_error("Minishell: cd: HOME not set\n");
		return (NULL);
	}
	return (home->value);
}

void	builtin_cd(char **args)
{
	char	cwd[PATH_MAX];
	char	*new_wd;

	if (!args || !*args)
	{
		new_wd = get_home_dir();
		if (new_wd)
			chdir(new_wd);
		return ;
	}
	if (ft_strcmp(args[0], "~") == 0)
	{
		new_wd = get_home_dir();
		if (new_wd)
			chdir(new_wd);
	}
	else if (ft_strcmp(args[0], "-") == 0)
		chdir(find_env_var("OLDPWD", g_shell.env_list)->value);
	else if (ft_strcmp(args[0], "..") == 0)
	{
		getcwd(cwd, PATH_MAX);
		new_wd = ft_substr(cwd, 0, ft_strlen(cwd) - ft_strlen(ft_strrchr(cwd, '/')));
		chdir(new_wd);
		free(new_wd);
	}
	else if (args[1])
		pop_error("Minishell: cd: too many arguments\n");
	else
	{
		if (chdir(args[0]) == -1)
		{
			pop_error("Minishell :");
			pop_error(args[0]);
			pop_error(": No such file or directory\n");
		}
	}
}
