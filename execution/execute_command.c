/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: m3ayz00 <m3ayz00@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 17:52:45 by msaadidi          #+#    #+#             */
/*   Updated: 2024/07/20 23:20:45 by m3ayz00          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_path(char *cmd, char *env_path)
{
	char	**paths;
	char	*tmp;
	int		i;

	paths = ft_split(env_path, ":");
	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		tmp = ft_strjoin(tmp, cmd);
		if (!access(tmp, F_OK | X_OK))
			return (tmp);
		i++;
	}
	return (NULL);
}

static char	*get_cmd_path(char *cmd)
{
	char	*path;
	char	*tmp;

	if (minishell_error(cmd))
		return (NULL);
	path = get_value("PATH");
	if (path)
	{
		tmp = check_command(cmd, path);
		if (tmp)
			return (tmp);
		return (NULL);
	}
	else
		return (check_validity(cmd));
	print_error(cmd, "command not found");
	mshell()->exit_status = 127;
	return (NULL);
}

void	actual_command(t_tree *root, char *cmd_path)
{
	if (cmd_path && execve(cmd_path, root->argv, get_current_env_array()) == -1)
		exit(EXIT_FAILURE);
	else
		exit(EXIT_SUCCESS);
}

void	prepare_command(t_tree *root, char **cmd_path)
{
	int	ext;

	if (root->redir_list)
		handle_redirections(root->redir_list, &ext, 0);
	if (root->argv)
	{
		(*cmd_path) = get_cmd_path(root->argv[0]);
		if (!(*cmd_path))
			exit(mshell()->exit_status);
		else if (is_directory((*cmd_path)))
		{
			print_error((*cmd_path), "command not found");
			mshell()->exit_status = 127;
			exit(mshell()->exit_status);
		}
	}
	actual_command(root, *cmd_path);
}

void	dupping(int is_builtin, t_redir *current, int index, int std_fd)
{
	if (is_builtin && !current->next)
		dup2(current->fds[index], std_fd);
	else if (!is_builtin)
		dup2(current->fds[index], std_fd);
}
