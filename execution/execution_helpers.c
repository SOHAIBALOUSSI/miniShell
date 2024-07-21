/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 18:04:54 by sait-alo          #+#    #+#             */
/*   Updated: 2024/07/19 17:08:01 by msaadidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_builtin(char *cmd)
{
	return (ft_strcmp(cmd, "cd") == 0 || ft_strcmp(cmd, "echo") == 0
		|| ft_strcmp(cmd, "env") == 0 || ft_strcmp(cmd, "pwd") == 0
		|| ft_strcmp(cmd, "export") == 0 || ft_strcmp(cmd, "unset") == 0
		|| ft_strcmp(cmd, "exit") == 0);
}

int	is_directory(const char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) == 0)
		return (S_ISDIR(path_stat.st_mode));
	return (0);
}

char	*get_last_arg(char **args)
{
	int	i;

	if (!args || !*args)
		return (NULL);
	i = 0;
	while (args[i + 1])
		i++;
	return (args[i]);
}

void	set_dollar_(char *key, char *value)
{
	t_env	*env;

	env = find_env_var(key, *mshell()->env_list);
	if (env)
		env->value = ft_strdup(value);
	else
		append_env(mshell()->env_list,
			create_env(ft_strjoin(key, ft_strjoin("=", value))));
}

char	*check_command(char *cmd, char *path)
{
	char	*tmp;

	if (ft_strchr(cmd, '/'))
		return (check_validity(cmd));
	else
	{
		if (is_directory(cmd))
		{
			mshell()->exit_status = 127;
			return (print_error(cmd, "command not found"), NULL);
		}
	}
	tmp = get_path(cmd, path);
	if (tmp)
		return (tmp);
	mshell()->exit_status = 127;
	return (print_error(cmd, "command not found"), NULL);
}
