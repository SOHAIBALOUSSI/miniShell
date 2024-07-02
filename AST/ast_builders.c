#include "../minishell.h"

t_tree	*create_op_node(e_tok type)
{
	t_tree	*op;

	op = m_alloc(sizeof(t_tree), ALLOC);
	op->type = type;
	op->left = NULL;
	op->right = NULL;
	return (op);
}

t_redir	*create_redir_node(e_tok type, char *file_name, size_t length)
{
	t_redir	*redir;

	redir = m_alloc(sizeof(t_redir), ALLOC);
	redir->type = type;
	redir->file_name = ft_strndup(file_name, length);
	redir->next = NULL;
	redir->fds[0] = -1;
	redir->fds[1] = -1;
	return (redir);
}


t_tree	*create_cmd_node(void)
{
	t_tree	*cmd;

	cmd = m_alloc(sizeof(t_tree), ALLOC);
	cmd->type = _CMD;
    cmd->argc = 0;
	cmd->argv = NULL;
	cmd->redir_list = NULL;
	return (cmd);
}

t_tree *create_subshell_node(t_tree *subshell)
{
	t_tree	*cmd;

	cmd = m_alloc(sizeof(t_tree), ALLOC);
	cmd->type = _SUBSHELL;
	cmd->subtree = subshell;
	cmd->redir_list = NULL;
	return (cmd);
}

t_tree	*create_pipe_node(t_token **tokens)
{
	t_tree	*pipe;

	pipe = m_alloc(sizeof(t_tree), ALLOC);
	pipe->type = _PIPE;
	pipe->pipe_line = NULL;
	pipe->pipe_count = 0;
	return (pipe);
}
