/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 16:08:37 by sait-alo          #+#    #+#             */
/*   Updated: 2024/07/19 17:08:01 by msaadidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_tree	*parse_redirection(t_token **tokens, t_tree *cmd)
{
	char	*heredocfilename;
	t_tok	redir_type;
	t_redir	*redir;
	t_redir	*last_redir;

	heredocfilename = NULL;
	redir_type = (*tokens)->type;
	heredocfilename = (*tokens)->heredoc_file;
	*tokens = (*tokens)->next;
	redir = create_redir_node(*tokens, redir_type, heredocfilename);
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

static t_tree	*parse_cmd(t_token **tokens)
{
	t_tree	*cmd;
	t_token	*tmp;

	tmp = *tokens;
	cmd = create_cmd_node();
	while (tmp)
	{
		if (is_word(tmp->type))
			add_arg_to_cmd(cmd, tmp->location.location, tmp->location.length);
		else if (is_redirection(tmp->type))
			cmd = parse_redirection(&tmp, cmd);
		else
			break ;
		tmp = tmp->next;
	}
	*tokens = tmp;
	return (cmd);
}

static t_tree	*parse_subshell(t_token **tokens)
{
	t_tree	*subshell;
	t_tree	*cmd;

	*tokens = (*tokens)->next;
	subshell = parse_cmd_line(tokens);
	*tokens = (*tokens)->next;
	cmd = create_subshell_node(subshell);
	while (*tokens && is_redirection((*tokens)->type))
	{
		cmd = parse_redirection(tokens, cmd);
		if ((*tokens)->next)
			*tokens = (*tokens)->next;
	}
	return (cmd);
}

static t_tree	*parse_pipe_line(t_token **tokens)
{
	t_tree	*cmd;
	t_tree	*pipe;

	cmd = NULL;
	pipe = NULL;
	pipe = create_pipe_node();
	while (*tokens)
	{
		if ((*tokens)->type == _PAREN_OPEN)
			cmd = parse_subshell(tokens);
		else
			cmd = parse_cmd(tokens);
		add_cmd_to_pipeline(pipe, cmd);
		if (*tokens && (*tokens)->type == _PIPE)
		{
			mshell()->pipe_count++;
			*tokens = (*tokens)->next;
		}
		else
			break ;
	}
	if (pipe->pipe_count <= 1)
		return (cmd);
	pipe->pipe_line[pipe->pipe_count] = NULL;
	return (pipe);
}

t_tree	*parse_cmd_line(t_token **tokens)
{
	t_tree	*left;
	t_tree	*op;

	left = NULL;
	op = NULL;
	left = parse_pipe_line(tokens);
	while (*tokens && ((*tokens)->type == _AND || (*tokens)->type == _OR))
	{
		op = create_op_node((*tokens)->type);
		*tokens = (*tokens)->next;
		op->left = left;
		op->right = parse_pipe_line(tokens);
		left = op;
	}
	return (left);
}
