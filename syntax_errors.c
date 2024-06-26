#include "minishell.h"

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
	g_shell.closed_paren_count = 0;
	g_shell.open_paren_count = 0;
	g_shell.single_quote_count = 0;
	g_shell.double_quote_count = 0;
	g_shell.pipe_count = 0;
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
	if (g_shell.single_quote_count % 2 != 0 || g_shell.double_quote_count % 2 != 0)
		return (pop_error("Minishell: syntax error 'unclosed parenthesis'\n"), 0);
	else if (g_shell.closed_paren_count != g_shell.open_paren_count)
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
            break;
        }
        ft_putendl_fd(line, fd);
        m_free(line);
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
		return (perror("open"), exit(1), NULL); // TODO: free memory before exit and set exit status to 1
	signal(SIGINT, SIG_IGN); // signals to handle later
	write_to_heredoc(fd, delimiter);
	close(fd);
	m_free(delimiter);
	return (heredoc_filename);
}

char	*get_heredoc_filename(t_token *current)
{
	char	*heredoc_filename;

	current->delimiter = ft_strndup(current->next->location.location, current->next->location.length);
	heredoc_filename = read_heredoc(current->delimiter);
	return (heredoc_filename);
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
		current->heredoc_file = get_heredoc_filename(current);
		// printf("%s\n", current->heredoc_file);
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
            || !check_parentheses(current) || !check_word(current))
        {
            return (EXIT_FAILURE);
        }
        current = current->next;
    }
    if (!check_quotes_and_parens())
        return (EXIT_FAILURE);
    return (EXIT_SUCCESS);
}