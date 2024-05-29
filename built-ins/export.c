#include "../minishell.h"

static void	update_env_var_value(t_env *var, char *new_value, bool is_add)
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

static char	**sort_array(char **arr)
{
	int i;
	char *tmp;

	i = 0;
	while (arr[i])
	{
		if (arr[i + 1] && ft_strcmp(arr[i], arr[i + 1]) >= 0)
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

static char	**lst_to_arr(t_env **env_list)
{
	int		i;
	t_env	*ptr;
	char	**arr;

	i = 0;
	arr = m_alloc((lst_size(env_list) + 1) * sizeof(char *), ALLOC);
	ptr = *env_list;
	while (ptr)
	{
		if (ft_strcmp(ptr->key, "_") == 0)
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

static void	put_sorted_env(void)
{
	int		i;
	char	*put;
	char	**env_array;

	i = 0;
	env_array = lst_to_arr(&g_shell.env_list);
	env_array = sort_array(env_array);
	while (env_array[i])
	{
		printf("declare -x ");
		put = get_value(env_array[i]);
		if (put)
		{
			printf("%s", env_array[i]);
			printf("=\"");	
			printf("%s\"\n", put);
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
		return (put_sorted_env());
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
// 	if (ft_strcmp(av[1], "cd"))
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