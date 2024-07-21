/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 15:17:24 by sait-alo          #+#    #+#             */
/*   Updated: 2024/07/19 17:08:01 by msaadidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_minishell	*mshell(void)
{
	static t_minishell	shell;
	static t_env		*env_list;

	if (!shell.env_list)
		shell.env_list = &env_list;
	return (&shell);
}

void	process_line(char *line)
{
	t_token	*token_lst;
	t_tree	*root;

	token_lst = tokenize_line(line);
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

void	shell_loop(void)
{
	char	*line;

	line = readline(SHELL_PROMPT);
	if (!line)
		return (write(2, "exit\n", 6), exit_clean(mshell()->exit_status));
	if (line[0] && check_spaces(line))
		add_history(line);
	process_line(line);
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
		shell_loop();
	exit_clean(mshell()->exit_status);
	return (EXIT_SUCCESS);
}
