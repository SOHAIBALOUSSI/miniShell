#include "minishell.h"

bool	is_valid_var(char *s)
{
	int i;

	i = 0;
	if (!ft_isalpha(s[i]))
		return (false);
	while (s[i] && s[i] != '+' && s[i] != '=')
	{
		if (!ft_isalnum(s[i]) && s[i] != '_' )
			return (false);
		i++;
	}
	if ((s[i] == '+' && s[i + 1] != '='))
		return false;
	return (true);
}

bool	is_exist(char *s)
{
	int i = 0;
	t_env *tmp;

	tmp = g_shell.env_list;
	while (tmp)
	{
		if (!ft_strncmp(tmp->key, s, ft_strlen(s)))
			return (true);
		tmp = tmp->next;
	}
	return (false);
}


void	export(char **args)
{
	// check for += || =
		// pop error in case invalid format
	/*	if its += we need to appeand the new value to 'key' if exist
			if its not exist we need to create it with the given value
		else if = we need to change the value of 'key' if exist
			if its not exist we need to create it with the given value

		append the created env to env_list
	*/
	int i = 0;
	char *key;
	char *value;

	while (args[i])
	{
		key = get_key(args[i]);
		value = ft_strchr(args[i], '=');
		if (is_valid_var(key))
		{
			if (is_exist(key))
			{
				search_and_change(&g_shell.env_list, key, value);
				i++;
				continue;
			}
			append_env(&g_shell.env_list, create_env(args[i]));
		}
		else
			pop_error("not valid var\n");
		i++;
	}
}

// int    pwd(void)
// {
// 	char *pwd;
// 	pwd = getenv("PWD");
// 	if (!pwd)
// 		return(printf("PWD envirment variable not found\n"), -1);
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
// 	printf("%s\n", get_var("value+="));
// }