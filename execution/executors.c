/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executors.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 18:07:19 by msaadidi          #+#    #+#             */
/*   Updated: 2024/07/19 17:08:01 by msaadidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	execute_operator(t_tree *operator)
{
	int	status;

	status = execute_ast(operator->left);
	if ((operator->type == _AND && status == 0)
		|| (operator->type == _OR && status != 0))
		return (execute_ast(operator->right));
	return (status);
}

int	execute_subshell(t_tree *root)
{
	pid_t	pid;
	int		status;
	int		ext;

	pid = fork();
	if (pid < 0)
	{
		pop_error("fork");
		return (1);
	}
	else if (pid == 0)
	{
		if (root->redir_list)
			handle_redirections(root->redir_list, &ext, 0);
		mshell()->exit_status = execute_ast(root->subtree);
		exit(mshell()->exit_status);
	}
	else
	{
		waitpid(pid, &status, 0);
		mshell()->exit_status = WEXITSTATUS(status);
	}
	return (mshell()->exit_status);
}

int	execute_cmd(t_tree *root)
{
	char	*cmd_path;
	pid_t	pid;
	int		status;

	cmd_path = NULL;
	expander(root);
	if (root->argv && root->argv[0] && is_builtin(root->argv[0]))
		return (execute_builtin(root));
	handle_p_signals();
	pid = fork();
	if (pid == 0)
		prepare_command(root, &cmd_path);
	else if (pid < 0)
		return (pop_error("Fork failed\n"), 1);
	else
	{
		mshell()->in_exec = 1;
		waitpid(pid, &status, 0);
		assign_exit(status);
	}
	return (handle_signals(), mshell()->exit_status);
}

int	execute_pipeline(t_tree **pipeline, int n_cmd)
{
	int	saved_output;
	int	saved_input;
	int	result;

	if (!pipeline || !(*pipeline))
		return (1);
	saved_output = dup(STDOUT_FILENO);
	saved_input = dup(STDIN_FILENO);
	result = actual_pipeline(pipeline, n_cmd);
	dup2(saved_input, STDIN_FILENO);
	close(saved_input);
	dup2(saved_output, STDOUT_FILENO);
	close(saved_output);
	return (result);
}

int	execute_ast(t_tree *root)
{
	if (mshell()->hd_interrupt)
	{
		mshell()->hd_interrupt = 0;
		return (mshell()->exit_status);
	}
	if (!root)
		return (1);
	else if (root->type == _AND || root->type == _OR)
		mshell()->exit_status = execute_operator(root);
	else if (root->type == _SUBSHELL)
		mshell()->exit_status = execute_subshell(root);
	else if (root->type == _PIPE)
		mshell()->exit_status = execute_pipeline(root->pipe_line,
			count_pipes(root->pipe_line));
	else if (root->type == _CMD)
		mshell()->exit_status = execute_cmd(root);
	if (root->redir_list)
		restore_redirections(root->redir_list);
	return (mshell()->exit_status);
}
