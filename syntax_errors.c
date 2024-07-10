#include "minishell.h"

void	heredoc_handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	exit(mshell()->exit_status);
}

void	handle_heredoc_signals(void)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTERM, SIG_IGN);
	signal(SIGINT, heredoc_handler);
}


static void	syntax_err(t_token *current)
{
	char	*err;

	if (current == NULL)
		err = ft_strdup("newline");
	else
		err = ft_substr(current->location.location, 0, current->location.length);
	ft_putstr_fd("Minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd(err, 2);
	ft_putstr_fd("\'\n", 2);
	mshell()->closed_paren_count = 0;
	mshell()->open_paren_count = 0;
	mshell()->single_quote_count = 0;
	mshell()->double_quote_count = 0;
	mshell()->pipe_count = 0;
	mshell()->heredoc_count = 0;
	m_free(err);
}

static int	check_pipe_and(t_token *current)
{
	if (is_pipe_or_and(current->type))
	{
		if ((!current->prev || !current->next)
			|| (is_pipe_or_and(current->prev->type)))
		{
			syntax_err(current);
			return (0);
		}
	}
	return (1);
}

static int	check_redirection(t_token *current)
{
	if (is_redirection(current->type))
	{
		if ((!current->next || !is_word(current->next->type)))
		{
			syntax_err(current->next);
			return (0);
		}
	}
	return (1);
}

static int	check_parentheses(t_token *current)
{
	if (current->type == _PAREN_OPEN && current->next)
	{
		if (is_pipe_or_and(current->next->type)
			|| current->next->type == _PAREN_CLOSED)
		{
			syntax_err(current->next);
			return (0);
		}
	}
	else if (current->type == _PAREN_CLOSED && current->next)
	{
		if (current->next->type == _PAREN_OPEN
			|| current->next->type == _WORD)
		{
			syntax_err(current->next);
			return (0);
		}
	}
	return (1);
}

static int	check_word(t_token *current)
{
	if (is_word(current->type) && current->next)
	{
		if (current->next->type == _PAREN_OPEN)
		{
			if (!current->prev || !is_word(current->prev->type))
				syntax_err(current->next->next);
			else
				syntax_err(current->next);
			return (0);
		}
	}
	return (1);
}

static int	check_quotes_and_parens(void)
{
	if (mshell()->single_quote_count % 2 != 0 || mshell()->double_quote_count % 2 != 0)
		return (pop_error("Minishell: syntax error 'unclosed parenthesis'\n"), 0);
	else if (mshell()->closed_paren_count != mshell()->open_paren_count)
		return (pop_error("Minishell: syntax error 'unclosed parenthesis'\n"), 0);
	return (1);
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
                m_free(line);
            break;
        }
        ft_putendl_fd(line, fd);
        m_free(line);
    }
}

void	here_doc(int fd, char *delimiter)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
	{
		pop_error("Fork failed\n");
		return ;
	}
	else if (pid == 0)
	{
		handle_heredoc_signals();
		write_to_heredoc(fd, delimiter);
		close(fd);
		exit(EXIT_SUCCESS);
	}
	else
	{
		waitpid(pid, &status, 0);
		close(fd);
		m_free(delimiter);
		if (WIFEXITED(status))
            mshell()->exit_status = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
        {
            mshell()->exit_status = 128 + WTERMSIG(status);
            if (WTERMSIG(status) == SIGINT)
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
char	*remove_quotes(char *str)
{
	int		i;
	int		j;
	bool	in_squote;
	bool	in_dquote;
	char	*new_str;

	i = 0;
	j = 0;
	in_squote = false;
	in_dquote = false;
	new_str = m_alloc(ft_strlen(str) + 1 * sizeof(char), ALLOC);
	mshell()->expand_oho = 1;
	while (str[i])
	{
		if (str[i] == '\'' && !in_dquote)
			in_squote = !in_squote;
		else if (str[i] == '\"' && !in_squote)
			in_dquote = !in_dquote;
		else
			new_str[j++] = str[i];
		i++;
	}
	new_str[j] = '\0';
	m_free(str);
	return (new_str);
}
bool	has_quotes(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == SQUOTE || str[i] == DQUOTE)
			return (true);
		i++;
	}
	return (false);
}
static int check_heredoc(t_token *current)
{
    if (current->type == _HEREDOC)
    {
        if (!current->next || !is_word(current->next->type))
        {
            syntax_err(current->next);
            return (0);
        }
		current->delimiter = ft_strndup(current->next->location.location, current->next->location.length);
		if (has_quotes(current->delimiter))
			current->delimiter = remove_quotes(current->delimiter);
		current->heredoc_file = read_heredoc(current->delimiter);
    }
    return (1);
}

int catch_syntax_errors(t_token *token_lst)
{
    t_token *current;

    current = token_lst;
    while (current)
    {
        if (!check_pipe_and(current) || !check_redirection(current)
            || !check_parentheses(current) || !check_word(current)
			|| !check_heredoc(current))
        {
			mshell()->exit_status = 2;
            return (EXIT_FAILURE);
        }
        current = current->next;
    }
    if (!check_quotes_and_parens())
	{
		mshell()->exit_status = 2;
        return (EXIT_FAILURE);
	}
    return (EXIT_SUCCESS);
}