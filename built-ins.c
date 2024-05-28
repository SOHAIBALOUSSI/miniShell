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

size_t	lst_size(t_env **lst)
{
	t_env	*tmp;
	size_t	size;

	tmp = *lst;
	size = 0;
	while (tmp)
	{
		if (strcmp(tmp->key, "_") == 0)
		{
			tmp = tmp->next;
			continue;
		}
		size++;
		tmp = tmp->next;
	}
	return (size);

}

char	**sort_array(char **arr)
{
	int i;
	char *tmp;

	i = 0;
	while (arr[i])
	{
		if (arr[i + 1] && strcmp(arr[i], arr[i + 1]) >= 0)
		{
			tmp = arr[i];
			arr[i] = arr[i + 1];
			arr[i + 1] = tmp;
			i = 0;
		}
		else
			i++;
	}
	return (arr);
}

char	**lst_to_arr(t_env **env_list)
{
	int		i;
	t_env	*ptr;
	char	**arr;

	i = 0;
	arr = m_alloc((lst_size(env_list) + 1) * sizeof(char *), ALLOC);
	ptr = *env_list;
	while (ptr)
	{
		if (strcmp(ptr->key, "_") == 0)
		{
			ptr = ptr->next;
			continue;
		}
		arr[i] = ptr->key;
		i++;
		ptr = ptr->next;
	}
	arr[i] = NULL;
	return (arr);
}

void	put_sorted_env()
{
	int		i;
	t_env	*put;
	char	**env_array;

	i = 0;
	env_array = lst_to_arr(&g_shell.env_list);
	env_array = sort_array(env_array);
	while (env_array[i])
	{
		printf("declare -x ");
		printf("%s", env_array[i]);
		put = find_env_var(env_array[i], g_shell.env_list);
		if (put)
		{
			printf("=\"");	
			printf("%s\"\n", put->value);
		}
		else
			printf("\n");
		i++;
	}
}

void	export(char **args)
{
	t_env	*var;
	t_env	*existing_var;
	
	if (!args)
		put_sorted_env();
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