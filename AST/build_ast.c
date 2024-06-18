#include "../minishell.h"

size_t	count_pipes(t_token **tokens);
t_tree	*parse_cmd_line(t_token **tokens);
t_tree	*parse_redirection(t_token **tokens, t_tree *cmd);

t_tree	*init_pipe_node(t_token **tokens)
{
	t_tree	*pipe;

	pipe = m_alloc(sizeof(t_tree), ALLOC);
	g_shell.pipe_count = count_pipes(tokens);
	pipe->type = _PIPE;
	pipe->left = NULL;
	pipe->right = NULL;
	pipe->pipe_line = m_alloc((sizeof(t_tree *) * (g_shell.pipe_count + 2)), ALLOC);
	return (pipe);
}

t_tree	*init_cmd_node(void)
{
	t_tree	*cmd;

	cmd = m_alloc(sizeof(t_tree), ALLOC);
	cmd->type = _CMD;
	cmd->argv = NULL;
	cmd->redir_list = NULL;
	return (cmd);
}

size_t	count_pipes(t_token **tokens)
{
	t_token	*tmp;
	size_t	count;

	count = 0;
	tmp = *tokens;
	while (tmp)
	{
		if (tmp->type == _PIPE)
			count++;
		if (tmp->type == _AND || tmp->type == _OR || tmp->type == _PAREN_CLOSED)
			break ;
		tmp = tmp->next;
	}
	return (count);
}

t_tree *parse_subshell(t_token **tokens) {
    t_tree *subshell;
    t_tree *cmd;

    if (!(*tokens) || (*tokens)->type != _PAREN_OPEN)
        return (NULL);
    *tokens = (*tokens)->next;
    subshell = m_alloc(sizeof(t_tree), ALLOC);
    subshell->type = _SUBSHELL;
	subshell->pipe_line = NULL;
    subshell->subtree = parse_cmd_line(tokens);
    if (!(*tokens) || (*tokens)->type != _PAREN_CLOSED)
        return (NULL);
    *tokens = (*tokens)->next;
    while (*tokens && is_redirection((*tokens)->type))
        subshell = parse_redirection(tokens, subshell);
    return (subshell);
}


size_t	count_cmds(t_token **tokens)
{
	size_t	count;
	t_token	*tmp;

	count = 0;
	tmp = *tokens;
	while (tmp)
	{
		if (!tmp->prev || (tmp->prev && is_word(tmp->type)
				&& !is_redirection(tmp->prev->type)))
			count++;
		if (tmp->type == _AND || tmp->type == _OR)
			break ;
		tmp = tmp->next;
	}
	return (count);
}

char	*ft_strndup(char *s1, size_t n)
{
	size_t	i;
	char	*new;

	i = 0;
	new = m_alloc(sizeof(char) * (n + 1), ALLOC);
	while (s1[i] && i < n)
	{
		new[i] = s1[i];
		i++;
	}
	new[i] = '\0';
	return (new);
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

t_tree	*parse_redirection(t_token **tokens, t_tree *cmd)
{
	e_tok	redir_type;
	t_redir	*redir;
	t_redir	*last_redir;

	redir_type = (*tokens)->type;
	*tokens = (*tokens)->next;
	redir = create_redir_node(redir_type, (*tokens)->location.location,
			(*tokens)->location.length);
	if (!cmd->redir_list)
		cmd->redir_list = redir;
	else
	{
		last_redir = cmd->redir_list;
		while (last_redir->next)
			last_redir = last_redir->next;
		last_redir->next = redir;
	}
	return (cmd);
}

t_tree	*parse_cmd(t_token **tokens)
{
	size_t	idx;
	t_tree	*cmd;
	t_token	*tmp;

	idx = 0;
	tmp = *tokens;
	cmd = init_cmd_node();
	cmd->argv = m_alloc(sizeof(char *) * (count_cmds(&tmp) + 1), ALLOC);
	while (tmp)
	{
		if (is_word(tmp->type))
			cmd->argv[idx++] = ft_strndup(tmp->location.location,
					tmp->location.length);
		else if (is_redirection(tmp->type))
			cmd = parse_redirection(&tmp, cmd);
		else
			break ;
		tmp = tmp->next;
	}
	cmd->argv[idx] = NULL;
	*tokens = tmp;
	return (cmd);
}

t_tree	*parse_pipe(t_token **tokens)
{
	int		curr_cmd;
	t_tree	*pipe;
	t_tree	*cmd;

	curr_cmd = 0;
	pipe = init_pipe_node(tokens);
	while (*tokens)
	{
		cmd = parse_subshell(tokens);
		if (!cmd)
			cmd = parse_cmd(tokens);
		pipe->pipe_line[curr_cmd++] = cmd;
		if (!*tokens || (*tokens)->type != _PIPE)
			break ;
		*tokens = (*tokens)->next;
	}
	pipe->pipe_line[curr_cmd] = NULL;
	if (g_shell.pipe_count == 0)
		return (cmd);
	return (pipe);
}

t_tree	*create_op_node(e_tok type)
{
	t_tree	*op;

	op = m_alloc(sizeof(t_tree), ALLOC);
	op->type = type;
	op->left = NULL;
	op->right = NULL;
	return (op);
}

t_tree	*parse_cmd_line(t_token **tokens)
{
	t_tree	*left;
	t_tree	*right;
	t_tree	*op;

	left = parse_pipe(tokens);
	if (!*tokens)
		return (left);
	while (*tokens)
	{
		if (!*tokens || (*tokens)->type == _PAREN_CLOSED
			|| (*tokens)->type != _AND || (*tokens)->type != _OR)
			break ;
		op = create_op_node((*tokens)->type);
		*tokens = (*tokens)->next;
		op->left = left;
		op->right = parse_pipe(tokens);
		left = op;
	}
	return (left);
}

t_tree	*parser(t_token *tokens)
{
	t_tree	*root;

	root = parse_cmd_line(&tokens);
	if (!root)
		return (NULL);
	return (root);
}