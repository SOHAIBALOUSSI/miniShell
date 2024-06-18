/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soutchak <soutchak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 12:00:00 by soutchak         #+#    #+#             */
/*   Updated: 2024/06/14 12:00:00 by soutchak        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "memory.h"

t_exec	*init_exec_node(void)
{
	t_exec	*node;

	node = ft_malloc(sizeof(t_exec));
	node->type = EXEC;
	node->argv = NULL;
	node->argc = 0;
	return (node);
}

t_block	*init_block_node(t_tree *child)
{
	t_block	*node;

	node = ft_malloc(sizeof(t_block));
	node->type = BLOCK;
	node->child = child;
	return (node);
}

t_pipe	*init_pipe_node(void)
{
	t_pipe	*node;

	node = ft_malloc(sizeof(t_pipe));
	node->type = PIPE;
	node->nodes = NULL;
	node->nb_pipes = 0;
	return (node);
}

t_and_or *init_and_or_node(t_etype type, t_tree *left)
{
	t_and_or *node;

	node = ft_malloc(sizeof(t_and_or));
	node->type = type;
	node->left = left;
	node->right = NULL;
	return (node);
}

t_tree	*parse_cmd(t_token **tokens)
{
	t_exec	*exec;
	t_tree	*node;

	exec = init_exec_node();
	node = (t_tree *)exec;
	while (*tokens && (*tokens)->type & (STRING | REDIR))
	{
		if ((*tokens)->type == STRING)
			add_arg(&exec->argv, &exec->argc, (*tokens)->value);
		else if ((*tokens)->type & REDIR)
			node = parse_redir(tokens, node);
		if (!*tokens)
			break;
		*tokens = (*tokens)->next;
	}
	return (node);
}

t_tree	*parse_subshell(t_token **tokens)
{
	t_tree	*subshell;
	t_block	*block;

	if (!*tokens || (*tokens)->type != LPAR)
		return (NULL);
	*tokens = (*tokens)->next;
	subshell = parse_sequence(tokens);
	if (!*tokens || (*tokens)->type != RPAR)
		return (NULL);
	*tokens = (*tokens)->next;
	block = init_block_node(subshell);
	return ((t_tree *)block);
}

t_tree	*parse_pipe(t_token **tokens)
{
	t_tree	*left;
	t_tree	*right;
	t_pipe	*pipe;

	left = parse_subshell(tokens);
	if (!left)
		left = parse_cmd(tokens);
	pipe = init_pipe_node();
	pipe->nodes = ft_realloc(pipe->nodes, 0, sizeof(t_tree *));
	pipe->nodes[pipe->nb_pipes++] = left;
	while (*tokens && (*tokens)->type == PIPE)
	{
		*tokens = (*tokens)->next;
		right = parse_subshell(tokens);
		if (!right)
			right = parse_cmd(tokens);
		pipe->nodes = ft_realloc(pipe->nodes, pipe->nb_pipes * sizeof(t_tree *),
								  (pipe->nb_pipes + 1) * sizeof(t_tree *));
		pipe->nodes[pipe->nb_pipes++] = right;
	}
	return ((t_tree *)pipe);
}

t_tree	*parse_sequence(t_token **tokens)
{
	t_tree	*left;
	t_tree	*right;
	t_and_or *node;

	left = parse_pipe(tokens);
	node = NULL;
	while (*tokens && (*tokens)->type & (AND | OR))
	{
		if (node)
		{
			right = parse_pipe(tokens);
			node->right = right;
			left = (t_tree *)node;
		}
		else
		{
			node = init_and_or_node((*tokens)->type, left);
			*tokens = (*tokens)->next;
			right = parse_pipe(tokens);
			node->right = right;
			left = (t_tree *)node;
		}
	}
	return (left);
}

void	parser(t_tree **tree, t_token *tokens)
{
	*tree = parse_sequence(&tokens);
}