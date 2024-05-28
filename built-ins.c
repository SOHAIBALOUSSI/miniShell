#include "minishell.h"

void	built_ins_err(char *err_key)
{
	ft_putstr_fd(RED, 2);
	ft_putstr_fd("Minishell: export: `", 2);
	ft_putstr_fd(err_key, 2);
	ft_putstr_fd("\': not a valid identifier\n", 2);
	ft_putstr_fd(RESTORE, 2);
}

bool	is_valid_key(char *s)
{
	int	i;

	i = 0;
	if (!ft_isalpha(s[i]))
		return (false);
	while (s[i] && s[i] != '=')
	{
		if (s[i] == '+')
		{
			if (s[i + 1] == '=')
			{
				g_shell.is_add = 1;
				return (true);
			}
			return (false);
		}
		if (!ft_isalnum(s[i]) && s[i] != '_' )
			return (false);
		i++;
	}
	return (true);
}

t_env	*find_env_var(char *key, t_env *env_list)
{
	t_env	*tmp;

	tmp = env_list;
	while (tmp)
	{
		if (strcmp(tmp->key, key) == 0)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

void	update_env_var_value(t_env *var, char *new_value, bool is_add)
{
	char	*tmp;

	if (is_add)
	{
		tmp = ft_strjoin(var->value, new_value);
		free(var->value);
		var->value = tmp;
	}
	else
	{
		free(var->value);
		var->value = ft_strdup(new_value);
	}
}
void	put_sorted_env()
{
	t_env	*put;
	
	put = g_shell.env_list;

	int alpha = 0;
	while (/* condition */)
	{
		/* code */
	}
	
	
}
void	export(char **args)
{
	t_env	*var;
	t_env	*existing_var;
	// if (!*args)
	// 	put_sorted_env();
	while (*args)
	{
		if (is_valid_key(*args))
		{
			var = create_env(*args);
			existing_var = find_env_var(var->key, g_shell.env_list);
			if (existing_var)
			{
				if (g_shell.is_add)
					update_env_var_value(existing_var, var->value, true);
				else
					update_env_var_value(existing_var, var->value, false);
			}
			else
				append_env(&g_shell.env_list, var);
		}
		else
			built_ins_err(*args);
		args++;
	}
}

// int    pwd(void)
// {
// 	char *pwd;
// 	pwd = getenv("PWD");
// 	if (!pwd)
// 		return(printf("PWD envirment keyiable not found\n"), -1);
// 	printf("%s\n", pwd);
// 	return 0;
// }

// void    change_dir(void)
// {
// 	char *current_dir;

// 	if (ac != 3)
// 		return (ft_putstr_fd("Minishell: cd :too many arguments\n", 2), -1);
// 	if (strcmp(av[1], "cd"))
// 		return (ft_putstr_fd("Minishell: not cd command!\n", 2), -1);
// 	current_dir = getcwd(NULL, PATH_MAX);
// 	if (!current_dir)
// 		return (-1);
// 	if (current_dir)
// 		printf("current dir is : [%s]\n", current_dir);
// 	if (!chdir(av[2]))
// 		printf("dir changed to : [%s]\n", av[2]);
// 	else
// 		printf("%s, dir not found", av[2]);
// 	return (-1);
// }


// int main()
// {
// 	printf("%s\n", get_key("value+="));
// }