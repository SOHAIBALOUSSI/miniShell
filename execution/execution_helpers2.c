/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_helpers2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: m3ayz00 <m3ayz00@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 18:03:19 by msaadidi          #+#    #+#             */
/*   Updated: 2024/07/20 22:58:54 by m3ayz00          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	close_pipes(int fd[][2], int n_cmd)
{
	int	i;

	i = 0;
	while (i < n_cmd)
	{
		close(fd[i][0]);
		close(fd[i][1]);
		i++;
	}
}

int	count_pipes(t_tree **pipe_line)
{
	int	i;

	i = 0;
	while (pipe_line[i])
		i++;
	return (i);
}

static int	execute_builtin2(t_tree *root)
{
	char	**argv;
	int		ret;

	argv = root->argv;
	ret = -1;
	if (ft_strcmp(argv[0], "pwd") == 0)
		ret = builtin_pwd();
	else if (ft_strcmp(argv[0], "export") == 0)
	{
		builtin_export(argv + 1);
		ret = EXIT_SUCCESS;
	}
	else if (ft_strcmp(argv[0], "unset") == 0)
		ret = builtin_unset(argv + 1);
	else if (ft_strcmp(argv[0], "exit") == 0)
		ret = builtin_exit(argv + 1);
	return (ret);
}

int	execute_builtin(t_tree *root)
{
	char	**argv;
	int		ret;

	ret = -1;
	argv = root->argv;
	if (root->redir_list)
		handle_redirections(root->redir_list, &ret, 1);
	if (ret == EXIT_FAILURE)
		return (ret);
	if (ft_strcmp(argv[0], "cd") == 0)
		ret = builtin_cd(argv + 1);
	else if (ft_strcmp(argv[0], "echo") == 0)
		ret = builtin_echo(argv + 1);
	else if (ft_strcmp(argv[0], "env") == 0)
		ret = builtin_env();
	else
		ret = execute_builtin2(root);
	restore_redirections(root->redir_list);
	return (ret);
}

int	alloc_pipe_fd(pid_t **pid, int (**fd)[2], int n_cmd)
{
	*pid = (pid_t *)m_alloc(sizeof(pid_t) * n_cmd, ALLOC);
	if (!(*pid))
		return (-1);
	(*fd) = (int (*)[2])m_alloc(sizeof(int [2]) * (n_cmd - 1), ALLOC);
	if (!(*fd))
		return (-1);
	return (0);
}
