/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
<<<<<<< HEAD
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 17:53:36 by msaadidi          #+#    #+#             */
/*   Updated: 2024/07/16 18:24:53 by msaadidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

=======
/*   By: sait-alo  <sait-alo@student.1337.ma >      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 11:16:37 by sait-alo          #+#    #+#             */
/*   Updated: 2024/07/16 15:35:33 by sait-alo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


>>>>>>> partner-repo3/main
#include "../minishell.h"

static int	pipe_it_up(int fd[][2], int n_cmd)
{
	int	i;

	i = 0;
	while (i < n_cmd - 1)
	{
		if (pipe(fd[i]) < 0)
		{
			perror("Minishell: ");
			return (close_pipes(fd, i), -1);
		}
		i++;
	}
	return (0);
}

void	wait_children(pid_t pid[], int n_cmd, int *status)
{
	int	i;

<<<<<<< HEAD
	i = 0;
	while (i < n_cmd)
	{
		waitpid(pid[i], status, 0);
		mshell()->exit_status = WEXITSTATUS(*status);
		i++;
	}
=======
    i = 0;
    while (i < n_cmd - 1)
    {
        if (pipe(fd[i]) < 0)
        {
            perror("Minishell: ");
            return (close_pipes(fd, i), -1);
        }
        i++;
    }
    return (0);
>>>>>>> partner-repo3/main
}

void	execute_process(t_tree **pipeline, int i, int *status)
{
	if (pipeline[i]->type == _SUBSHELL)
		(*status) = execute_subshell(pipeline[i]);
	else
		(*status) = execute_cmd(pipeline[i]);
	exit((*status));
}

void	redirect_process(int n_cmd, int *i, int fd[][2])
{
	if ((*i) > 0)
		dup2(fd[(*i) - 1][0], STDIN_FILENO);
	if ((*i) < n_cmd - 1)
		dup2(fd[(*i)][1], STDOUT_FILENO);
	close_pipes(fd, n_cmd - 1);
}

int	actual_pipeline(t_tree **pipeline, int n_cmd)
{
	pid_t	pid[n_cmd];
	int		fd[n_cmd - 1][2];
	int		status;
	int		i;

	if ((pipe_it_up(fd, n_cmd)) == -1)
		return (-1);
	i = 0;
	while (i < n_cmd)
	{
		pid[i] = fork();
		if (pid[i] < 0)
			return (perror("Minishell: "), close_pipes(fd, n_cmd), -1);
		else if (pid[i] == 0)
		{
			redirect_process(n_cmd, &i, fd);
			execute_process(pipeline, i, &status);
		}
		i++;
	}
	close_pipes(fd, n_cmd - 1);
	wait_children(pid, n_cmd, &status);
	mshell()->pipe_count = 0;
	return (mshell()->exit_status);
}
