#include "minishell.h"

t_minishell *mshell(void)
{
    static t_minishell shell;
    static t_env *env_list;
    
    if (!shell.env_list)
        shell.env_list = &env_list;
    return (&shell);
}

void	process_input(char *line)
{
	t_token	*token_lst;
	t_tree	*root;

	token_lst = tokenizer(line);
	if (mshell()->heredoc_count > 16)
		return (pop_error(HEREDOC_MAX_ERROR), exit_clean(2));
	if (token_lst)
	{
		if (catch_syntax_errors(token_lst))
			return ;
		root = parse_cmd_line(&token_lst);
		execute_ast(root);
	}
}

void	read_cmd(void)
{
	char	*line;

	line = readline(SHELL_PROMPT);
	if (!line)
		return (write(2, "exit\n", 6), exit_clean(0));
	if (line[0] && check_spaces(line))
		add_history(line);
	process_input(line);
	reset_counters();
	free(line);
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
	}
	m_alloc(0, FREE);
	exit_clean(mshell()->exit_status);
	return (EXIT_SUCCESS);
}
