#include "../minishell.h"

t_tree	*create_op_node(e_tok type)
{
	t_tree	*op;

	op = m_alloc(sizeof(t_tree), ALLOC);
	op->type = type;
	return (op);
}

t_redir	*create_redir_node(t_token *token, e_tok redir_type, char *heredocfilename)
{
	t_redir	*redir;

	redir = m_alloc(sizeof(t_redir), ALLOC);
	redir->type = redir_type;
	if (redir_type == _HEREDOC)
		redir->file_name = heredocfilename;
	else
		redir->file_name = ft_strndup(token->location.location, token->location.length);
	redir->fds[0] = -1;
	redir->fds[1] = -1;
	redir->original_in = -1;
	redir->original_out = -1;
	return (redir);
}

t_tree	*create_cmd_node(void)
{
	t_tree	*cmd;

	cmd = m_alloc(sizeof(t_tree), ALLOC);
	cmd->type = _CMD;
	return (cmd);
}

t_tree *create_subshell_node(t_tree *subshell)
{
	t_tree	*cmd;

	cmd = m_alloc(sizeof(t_tree), ALLOC);
	cmd->type = _SUBSHELL;
	cmd->subtree = subshell;
	return (cmd);
}

t_tree	*create_pipe_node(t_token **tokens)
{
	t_tree	*pipe;

	pipe = m_alloc(sizeof(t_tree), ALLOC);
	pipe->type = _PIPE;
	pipe->pipe_count = 0;
	pipe->pipe_line = NULL;
	return (pipe);
}
