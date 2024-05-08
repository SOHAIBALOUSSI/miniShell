/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hes-saqu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 13:33:00 by hes-saqu          #+#    #+#             */
/*   Updated: 2024/03/05 13:33:02 by hes-saqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	**get_paths(int ac, char ***ultimate, char **envp)
{
	int		i;
	char	**res;

	i = 0;
	res = (char **)malloc(sizeof(char *) * (ac + 1));
	if (!res)
		return (NULL);
	while (i < ac)
	{
		res[i] = get_path(envp, ultimate[i][0]);
		i++;
	}
	res[i] = NULL;
	return (res);
}

char	*get_path(char **paths, char *cmd)
{
	int		i;
	int		j;
	char	*cmd_path;
	char	*path_jnd;

	i = 0;
	j = 0;
	if (access(cmd, X_OK | F_OK) == 0)
		return (ft_strdup2(cmd));
	while (paths[i] != NULL && cmd[0] != '/' && cmd[0])
	{
		path_jnd = ft_strjoin(paths[i], "/");
		cmd_path = ft_strjoin(path_jnd, cmd);
		free(path_jnd);
		if (access(cmd_path, X_OK | F_OK) == 0)
			return (cmd_path);
		free(cmd_path);
		i++;
	}
	write(1, "command not found:", 18);
	ft_putstr_fd(1, cmd);
	write(1, "\n", 1);
	return (ft_strdup2(cmd));
}

void	free_it(char ***ultimate, char **paths, int **p, int n)
{
	int	i;
	int	j;

	i = 0;
	if (paths != NULL)
	{
		while (paths[i] != NULL)
			free(paths[i++]);
		free(paths);
	}
	i = 0;
	if (ultimate != NULL)
	{
		while (ultimate[i] != NULL)
		{
			j = 0;
			while (ultimate[i][j] != NULL)
				free(ultimate[i][j++]);
			free(ultimate[i++]);
		}
		free(ultimate);
	}
	free_it_2(p, n);
}

void	free_it_2(int **p, int n)
{
	int	i;

	i = 0;
	if (n != 0)
	{
		while (i < n)
			free(p[i++]);
		free(p);
	}
}
