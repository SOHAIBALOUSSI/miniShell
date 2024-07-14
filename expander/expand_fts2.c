#include "../minishell.h"

static char	**add_to_argv(char *expanded_arg, char ***expanded_argv)
{
	int	i;
	char **new_argv;

	i = 0;
	while ((*expanded_argv)[i])
		i++;
	new_argv = m_alloc(sizeof(char *) * (i + 2), ALLOC);
	i = 0;
	while ((*expanded_argv)[i])
	{
		new_argv[i] = (*expanded_argv)[i];
		i++;
	}
	new_argv[i] = expanded_arg;
	new_argv[i + 1] = NULL;
	m_free(*expanded_argv);
	return (new_argv);
}

static void	split_and_add_to_new_argv(char *expanded_arg, char ***expanded_argv)
{
	int		i;
	char	**split;

	split = ft_split(expanded_arg, " \n\t\v\t\r");
	i = 0;
	while (split[i])
	{
		add_to_new_argv(split[i], expanded_argv, false);
		i++;
	}
	m_free(split);
}

void	add_to_new_argv(char *expanded_arg, char ***expanded_argv, bool to_split)
{
	if (to_split == true)
		split_and_add_to_new_argv(expanded_arg, expanded_argv);
	else
		*expanded_argv = add_to_argv(expanded_arg, expanded_argv);
}
