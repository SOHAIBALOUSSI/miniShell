#include "../minishell.h"

void	process_handler(int sig)
{
	(void)sig;
	exit(130);
}

void	handle_process_signals(void)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTERM, SIG_IGN);
	signal(SIGINT, process_handler);
}

void write_to_heredoc(int fd, char *delimiter)
{
	char *line;

	while (1)
	{
		line = readline("> ");
		if (!ft_strcmp(line, delimiter))
		{
			if (line)
				free(line);
			break;
		}
		ft_putendl_fd(line, fd);
		free(line);
	}
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
		{
			handle_process_signals();
			write_to_heredoc(fd, delimiter);
			close(fd);
			exit(EXIT_SUCCESS);
		}
		else
		{
			waitpid(pid, &status, 0);
			close(fd);
			m_free(delimiter);
			mshell()->exit_status = WEXITSTATUS(status);
			printf("%d\n", mshell()->exit_status);
			if (mshell()->exit_status == 130)
				mshell()->hd_interrupt = 1;
		}
	}
}

char *read_heredoc(char *delimiter)
{
	static	int	heredoc = 1;
	char	*heredoc_filename;
	int		fd;
	char	*heredoc_number;

	heredoc_number = ft_itoa(heredoc++);
	heredoc_filename = ft_strjoin("/tmp/heredoc_", heredoc_number);
	m_free(heredoc_number);
	fd = open(heredoc_filename, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd < 0)
		return (pop_error("heredoc open failed"), NULL);
	here_doc(fd, delimiter);
	return (heredoc_filename);
}

