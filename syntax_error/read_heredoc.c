/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 15:32:22 by sait-alo          #+#    #+#             */
/*   Updated: 2024/07/19 17:08:01 by msaadidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	write_to_heredoc(int fd, char *delimiter)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!ft_strcmp(line, delimiter))
		{
			if (line)
				free(line);
			break ;
		}
		ft_putendl_fd(line, fd);
		free(line);
	}
}

void	heredoc_process(int *fd, char **delimiter)
{
	handle_process_signals();
	write_to_heredoc(*fd, *delimiter);
	close(*fd);
	exit(EXIT_SUCCESS);
}

void	here_doc(int fd, char *delimiter)
{
	pid_t	pid;
	int		status;

	if (mshell()->hd_interrupt == 0)
	{
		pid = fork();
		if (pid < 0)
		{
			pop_error("Fork failed\n");
			return ;
		}
		else if (pid == 0)
			heredoc_process(&fd, &delimiter);
		else
		{
			waitpid(pid, &status, 0);
			close(fd);
			m_free(delimiter);
			mshell()->exit_status = WEXITSTATUS(status);
			if (mshell()->exit_status == 130)
				mshell()->hd_interrupt = 1;
		}
	}
}

char	*read_heredoc(char *delimiter)
{
	static int	heredoc = 1;
	char		*heredoc_filename;
	int			fd;
	char		*heredoc_number;

	heredoc_number = ft_itoa(heredoc++);
	heredoc_filename = ft_strjoin("/tmp/heredoc_", heredoc_number);
	m_free(heredoc_number);
	fd = open(heredoc_filename, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd < 0)
		return (pop_error("heredoc open failed"), NULL);
	here_doc(fd, delimiter);
	return (heredoc_filename);
}
