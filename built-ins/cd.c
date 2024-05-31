#include "../minishell.h"

static char	*get_home_dir(void)
{
	t_env	*home;

	home = find_env_var("HOME", g_shell.env_list);
	if (!home)
		return (NULL);
	return (home->value);
}

static void	update_pwd_env(char *new_dir, char *old_dir)
{
	t_env	*pwd;
	t_env	*oldpwd;

	pwd = find_env_var("PWD", g_shell.env_list);
	if (pwd)
		pwd->value = ft_strdup(new_dir);
	else
	{
		pwd = m_alloc(sizeof(t_env), ALLOC);
		pwd->key = ft_strdup("PWD");
		pwd->value = ft_strdup(new_dir);
		append_env(&g_shell.env_list, pwd);
	}
	oldpwd = find_env_var("OLDPWD", g_shell.env_list);
	if (oldpwd)
		oldpwd->value = ft_strdup(old_dir);
	else
	{
		oldpwd = m_alloc(sizeof(t_env), ALLOC);
		oldpwd->key = ft_strdup("OLDPWD");
		oldpwd->value = ft_strdup(old_dir);
		append_env(&g_shell.env_list, oldpwd);
	}
}

void	builtin_cd(char **args)
{
	char	*new_dir;
	char	old_dir[PATH_MAX];

	getcwd(old_dir, PATH_MAX);
	if (!args || !*args)
	{
		new_dir = get_home_dir();
		if (!new_dir)
			pop_error("Minishell: cd: HOME not set\n");
	}
	else if (args[1])
		pop_error("Minishell: cd: too many arguments\n");
	else
		new_dir = args[0];
	if (new_dir)
	{
		if (chdir(new_dir) == -1)
		{
			pop_error("Minishell: cd: ");
			pop_error(args[0]);
			pop_error(": No such file or directory");
		}
		else
			update_pwd_env(new_dir, old_dir);
	}
}
