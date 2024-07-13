#include "minishell.h"
#include "libs/libft/libft.h"
#include <sys/types.h>
#include <sys/wait.h>

t_minishell *mshell(void)
{
    static t_minishell shell;
    static t_env *env_list;
    
    if (!shell.env_list)
        shell.env_list = &env_list;
    return (&shell);
}
void	read_cmd(void);
void	handle_signals(void);

void	handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	// rl_redisplay();
	mshell()->exit_status = 130;
}

void	handle_signals(void)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTERM, SIG_IGN);
	signal(SIGINT, handler);
}

int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r');
}

int     check_spaces(char *line)
{
	int i = 0;
	while (line[i])
	{
		if (!ft_isspace(line[i]))
			return (1);
		i++;
	}
	return (0);
}

void	reset_counters(void)
{
	mshell()->closed_paren_count = 0;
	mshell()->open_paren_count = 0;
	mshell()->single_quote_count = 0;
	mshell()->double_quote_count = 0;
	mshell()->pipe_count = 0;
	mshell()->heredoc_count = 0;
	mshell()->expand_oho = 0;
}

void	process_input(char *line)
{
	t_token	*token_lst;
	t_tree	*root;

	token_lst = tokenizer(line);
	if (mshell()->heredoc_count > 16)
	{
		pop_error(HEREDOC_MAX_ERROR);
		exit(1);
		return ;
	}
	if (token_lst)
	{
		if (!catch_syntax_errors(token_lst))
		{
			root = parse_cmd_line(&token_lst);
			if (!root)
				return ;
			execute_ast(root);
		}
	}
	reset_counters();
}

void	read_cmd(void)
{
	char	*line;

	line = readline(SHELL_PROMPT);
	if (!line)
		return (printf("exit"), exit(-1)); // free mem
	if (line[0] && check_spaces(line))
		add_history(line);
	process_input(line);
	// free(line);
}

int	main(int ac, char **av, char **env)
{
	(void)av;
	(void)ac;
	
	if (!isatty(STDIN_FILENO))
		return (printf(NOT_TTY), EXIT_FAILURE);
	get_env_list(env);
	handle_signals();
	while (true)
	{
		read_cmd();
		// m_alloc(0, FREE);
		// printf("counter %ld", mshell()->double_quote_count);
	}
	// rl_clear_history();
	return (EXIT_SUCCESS);
}
