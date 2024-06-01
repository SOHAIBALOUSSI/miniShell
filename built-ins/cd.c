#include "../minishell.h"

void	set_newpwd(char *new_dir)
{
	t_env	*pwd;

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
}

void	set_oldpwd(char *old_dir)
{
	t_env	*oldpwd;
	
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

static void	update_pwd_env(char *new_dir, char *old_dir)
{
	set_newpwd(new_dir);
	set_oldpwd(old_dir);
}

int	builtin_cd(char **args)
{
	char		*new_dir;
	char		old_dir[PATH_MAX];
	struct stat	dir_stat;

	getcwd(old_dir, PATH_MAX);
	if (!args || !*args)
	{
		new_dir = find_env_var("HOME", g_shell.env_list)->value;
		if (!new_dir)
			return (pop_error("Minishell: cd: HOME not set\n"), EXIT_FAILURE);
		return (chdir(new_dir), update_pwd_env(new_dir, old_dir), EXIT_SUCCESS);
	}
	else if (args[1])
		return (pop_error("Minishell: cd: too many arguments\n"), EXIT_FAILURE);
	new_dir = args[0];
	if (new_dir)
	{
		if (access(new_dir, F_OK) != 0)
			return (pop_error("Minishell: cd: No such file or directory\n"), EXIT_FAILURE);
		if (stat(new_dir, &dir_stat) == 0)
			if (S_ISDIR(dir_stat.st_mode) == 0)
				return (pop_error("Minishell : cd: Not a direcotory\n"), EXIT_FAILURE);
		if (access(new_dir, R_OK) != 0)
			return (pop_error("Minishell: Permission denied\n"), EXIT_FAILURE);
		return (chdir(new_dir), update_pwd_env(new_dir, old_dir), EXIT_SUCCESS);
	}
	return (EXIT_SUCCESS);
}
