#include "minishell.h"


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
	int a = pwd();
}