#include "minishell.h"


bool	is_valid_var(char *s)
{
	int i;

	i = 0;
	if (!ft_isalpha(s[i]))
		return (false);
	while (s[i])
	{
		if (s[i] == '+' && s[i] == '=')
			break;
		else if (!(s[i] == '+' && s[i] == '='))
			return (0);
		if (!ft_isalnum(s[i]) || s[i] != '_' )
			return (false);
		i++;
	}
	return (true);
}

// void	export(char **args)
// {
// 	// check for += || =
// 		// pop error in case invalid format
// 	/*	if its += we need to appeand the new value to 'key' if exist
// 			if its not exist we need to create it with the given value
// 		else if = we need to change the value of 'key' if exist
// 			if its not exist we need to create it with the given value

// 		append the created env to env_list
// 	*/
// 	int i = 0;
// 	while (args[i])
// 	{
// 		if (is_valid_var(args[i]))
// 		{
// 			if (is_exist(args[i]))
// 			{
// 				search_and_change();
// 				append_var()
// 			}
// 			create_and_append();
// 		}
// 	}
// }

int    pwd(void)
{
	char *pwd;
	pwd = getenv("PWD");
	if (!pwd)
		return(printf("PWD envirment variable not found\n"), -1);
	printf("%s\n", pwd);
	return 0;
}

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

int	main(int ac, char **av, char **env)
{
	printf("is valid = %d\n", is_valid_var("1"));
}