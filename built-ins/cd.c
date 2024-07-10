#include "../minishell.h"

static void	update_pwd_env(char *new_dir, char *old_dir)
{
	t_env	*pwd;
	t_env	*oldpwd;

	pwd = find_env_var("PWD", *mshell()->env_list);
	if (pwd)
		pwd->value = ft_strdup(getcwd(NULL, PATH_MAX));
	else
	{
		pwd = m_alloc(sizeof(t_env), ALLOC);
		pwd->key = ft_strdup("PWD");
		pwd->value = ft_strdup(new_dir);
		append_env(mshell()->env_list, pwd);
	}	
	oldpwd = find_env_var("OLDPWD", *mshell()->env_list);
	if (oldpwd)
		oldpwd->value = ft_strdup(old_dir);
	else
	{
		oldpwd = m_alloc(sizeof(t_env), ALLOC);
		oldpwd->key = ft_strdup("OLDPWD");
		oldpwd->value = ft_strdup(old_dir);
		append_env(mshell()->env_list, oldpwd);
	}
}

int	chdir_and_update_env(char *new_dir, char *old_dir)
{
	if (chdir(new_dir) != 0)
		return (perror("Minishell: cd: chdir"), EXIT_FAILURE);
	update_pwd_env(new_dir, old_dir);
	return (EXIT_SUCCESS);
}

int	go__home(char *old_dir)
{
	t_env	*home;

	home = find_env_var("HOME", *mshell()->env_list);
	if (!home)
		return (pop_error("Minishell: cd: HOME not set\n"), EXIT_FAILURE);
	return (chdir_and_update_env(home->value, old_dir));
}

int	builtin_cd(char **args)
{
	char		*new_dir;
	char		old_dir[PATH_MAX];
	struct stat	dir_stat;

	if (!*args)
		return (go__home(old_dir));
	if (!getcwd(old_dir, PATH_MAX))
		return  (perror("Minishell: cd: getcwd: "), EXIT_FAILURE);
	if (args[1])
		return (pop_error("Minishell: cd: too many arguments\n"), EXIT_FAILURE);
	new_dir = args[0];
	if (new_dir && *args[0])
	{
		if (access(new_dir, F_OK) != 0)
			return (perror("Minishell: cd "), EXIT_FAILURE);
		if (stat(new_dir, &dir_stat) != 0)
			return (perror("Minishell: cd: stat "), EXIT_FAILURE);
		if (S_ISDIR(dir_stat.st_mode) == 0)
			return (pop_error("Minishell: cd : Not a directory\n"), EXIT_FAILURE);
		if (access(new_dir, X_OK) != 0)
			return (perror("Minishell: cd "), EXIT_FAILURE);
		return (chdir_and_update_env(new_dir, old_dir));
	}
	return (EXIT_SUCCESS);
}
