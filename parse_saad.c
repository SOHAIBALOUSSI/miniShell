/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   consturtor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soutchak <soutchak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 08:27:27 by saad              #+#    #+#             */
/*   Updated: 2024/05/03 18:50:49 by soutchak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parser.h"
#include "../../inc/memory.h"

t_tree	*and_or_node(t_etype type, t_tree *left, t_tree *right)
{
	t_and_or	*node;

	node = ft_malloc(sizeof(t_and_or), PARSER);
	if (type != AND && type != OR)
		return (NULL);
	node->type = type;
	node->left = left;
	node->right = right;
	return ((t_tree *)node);
}

t_tree	*block_node(t_tree *child)
{
	t_block	*node;

	node = ft_malloc(sizeof(t_block), PARSER);
	node->child = child;
	node->type = BLOCK;
	return ((t_tree *)node);
}

t_tree	*redir_node(t_etype type, t_tree *child, char *s)
{
	t_redir	*node;

	node = ft_malloc(sizeof(t_redir), PARSER);
	node->type = type;
	node->expand = true;
	if (!set_filename(node, s, type))
		return (NULL);
	if (type == REDIR_IN || type == HEREDOC)
	{
		node->fd = 0;
		node->flags = O_RDONLY;
	}
	else if (type == REDIR_OUT || type == APPEND)
	{
		node->fd = 1;
		node->flags = O_WRONLY | O_CREAT;
		if (type == APPEND)
			node->flags |= O_APPEND;
		if (type == REDIR_OUT)
			node->flags |= O_TRUNC;
	}
	else
		return (NULL);
	node->child = child;
	return ((t_tree *)node);
}

t_tree	*pipe_node(void)
{
	t_pipe	*node;

	node = ft_malloc(sizeof(t_pipe), PARSER);
	node->type = PIPE;
	node->nb_pipes = 0;
	node->nodes = NULL;
	return ((t_tree *)node);
}

t_tree	*pipe_add_node(t_tree *pipe, t_tree *node)
{
	t_pipe	*pipe_node;

	if (!pipe || !node)
		return (NULL);
	pipe_node = (t_pipe *)pipe;
	pipe_node->nodes = ft_realloc(pipe_node->nodes, \
								sizeof(t_tree *) * pipe_node->nb_pipes, \
								sizeof(t_tree *) * (pipe_node->nb_pipes + 1));
	pipe_node->nodes[pipe_node->nb_pipes] = node;
	pipe_node->nb_pipes++;
	return (pipe);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soutchak <soutchak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/01 01:37:28 by soutchak          #+#    #+#             */
/*   Updated: 2024/05/13 17:39:24 by soutchak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parser.h"
#include "../../inc/memory.h"
#include "../../inc/globals.h"

static void	write_to_heredoc(int fd, char *delimiter)
{
	char		*line;
	int			*error;

	error = heredoc_error();
	line = readline(HEREDOC_PROMPT);
	while (line && ft_strcmp(line, delimiter) && *error == -1)
	{
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
		line = readline(HEREDOC_PROMPT);
		error = heredoc_error();
	}
	if (line)
		free(line);
	if (*error != -1)
	{
		dup2(*error, 0);
		close(*error);
	}
}

char	*read_heardoc(char *delimiter, bool *expand)
{
	static int	heredoc = 1;
	char		*filename;
	int			fd;
	char		*n;

	delimiter = remove_quotes(delimiter, expand);
	n = ft_itoa(heredoc++);
	filename = ft_strjoin(HEREDOC_FILENAME, n);
	ft_free(n, GENERAL);
	fd = open(filename, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd == -1)
		return (error("open heredoc", NULL), ft_free_heap(), exit(1), NULL);
	signal(SIGINT, heredoc_handler);
	write_to_heredoc(fd, delimiter);
	close(fd);
	ft_free(delimiter, GENERAL);
	ft_init_signals();
	return (filename);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soutchak <soutchak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 19:44:30 by soutchak          #+#    #+#             */
/*   Updated: 2024/05/13 17:21:26 by soutchak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parser.h"
#include "../../inc/memory.h"

static char	*get_literal(char *del, int *i)
{
	char	*tmp;
	char	c;
	int		j;

	c = del[(*i)++];
	j = *i;
	while (del[j] && del[j] != c)
		j++;
	tmp = ft_substr(del, *i, j - *i);
	*i = j + 1;
	return (tmp);
}

static char	*get_regular(char *del, int *i)
{
	char	*tmp;
	int		j;

	j = *i;
	while (del[j] && del[j] != '\'' && del[j] != '\"')
		j++;
	tmp = ft_substr(del, *i, j - *i);
	*i = j;
	return (tmp);
}

static char	*join_literal(char *joined, char *del, int *i)
{
	char	*tmp;
	char	*tmp2;

	tmp = get_literal(del, i);
	if (joined)
	{
		tmp2 = ft_strjoin(joined, tmp);
		ft_free(tmp, GENERAL);
		ft_free(joined, GENERAL);
		joined = tmp2;
	}
	else
		joined = tmp;
	return (joined);
}

static char	*join_regular(char *joined, char *del, int *i)
{
	char	*tmp;
	char	*tmp2;

	tmp = get_regular(del, i);
	if (joined)
	{
		tmp2 = ft_strjoin(joined, tmp);
		ft_free(tmp, GENERAL);
		ft_free(joined, GENERAL);
		joined = tmp2;
	}
	else
		joined = tmp;
	return (joined);
}

char	*remove_quotes(char *del, bool *expand)
{
	char	*joined;
	int		i;

	*expand = true;
	i = 0;
	joined = NULL;
	while (del[i])
	{
		if (del[i] == '\'' || del[i] == '\"')
		{
			*expand = false;
			joined = join_literal(joined, del, &i);
		}
		else
			joined = join_regular(joined, del, &i);
	}
	ft_free(del, GENERAL);
	return (joined);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soutchak <soutchak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 23:35:30 by soutchak          #+#    #+#             */
/*   Updated: 2024/05/06 21:07:14 by soutchak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parser.h"
#include "../../inc/memory.h"

void	parser(t_tree **tree, t_token *tokens)
{
	if (!tokens)
		return ;
	*tree = parse_sequence(&tokens);
	if (!*tree)
		error("Syntax error", "parser");
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soutchak <soutchak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 06:10:57 by soutchak          #+#    #+#             */
/*   Updated: 2024/05/02 19:15:34 by soutchak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parser.h"
#include "../../inc/memory.h"

t_exec	*init_exec(void)
{
	t_exec	*node;

	node = ft_malloc(sizeof(t_exec), PARSER);
	node->type = EXEC;
	node->argv = NULL;
	node->argc = 0;
	node->env = NULL;
	return (node);
}

void	parse_str(t_token *tmp, t_exec *node)
{
	node->argv = ft_realloc(node->argv, sizeof(char *) * (node->argc), \
							sizeof(char *) * (node->argc + 1));
	node->argv[node->argc] = ft_strndup(tmp->location.start, tmp->location.len);
	node->argc++;
}

t_tree	*parse_cmd_and_redirs(t_token **tokens, t_exec *node)
{
	t_token	*tmp;
	t_tree	*exec;

	exec = (t_tree *)node;
	tmp = *tokens;
	while (tmp)
	{
		if (tmp->type & STRING)
		{
			parse_str(tmp, node);
			tmp = tmp->next;
		}
		else if (tmp->type & REDIR)
		{
			exec = parse_redir(&tmp, exec);
			if (!exec)
				return (NULL);
		}
		else
			break ;
	}
	*tokens = tmp;
	return (exec);
}

t_redir	*get_last_redir(t_tree *tree)
{
	if (!tree || !(tree->type & REDIR))
		return (NULL);
	if (tree->type & REDIR && !(((t_redir *)tree)->child->type & REDIR))
		return ((t_redir *)tree);
	return (get_last_redir(((t_redir *)tree)->child));
}

char	*set_filename(t_redir *node, char *s, t_etype type)
{
	char	*filename;
	bool	expand;

	if (type != HEREDOC)
		return (node->file = s, s);
	expand = true;
	filename = read_heardoc(s, &expand);
	node->expand = expand;
	if (!filename)
		return (NULL);
	return (node->file = filename, node->file);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RDP_2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soutchak <soutchak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 18:50:49 by soutchak          #+#    #+#             */
/*   Updated: 2024/05/02 18:51:15 by soutchak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parser.h"
#include "../../inc/memory.h"

t_tree	*parse_sequence_tail(t_token **tokens, t_tree *left)
{
	t_tree	*right;
	t_etype	type;

	if (!(*tokens))
		return (left);
	else if (!((*tokens)->type & CTRL))
		return (NULL);
	type = (*tokens)->type;
	(*tokens) = (*tokens)->next;
	right = parse_pipe(tokens);
	if (!right)
		return (NULL);
	if (!(*tokens) || !((*tokens)->type & CTRL))
		return (and_or_node(type, left, right));
	left = and_or_node(type, left, right);
	return (parse_sequence_tail(tokens, left));
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RDP.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soutchak <soutchak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 18:49:15 by soutchak          #+#    #+#             */
/*   Updated: 2024/05/05 16:14:34 by soutchak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parser.h"
#include "../../inc/memory.h"

t_tree	*parse_exec(t_token **tokens)
{
	t_tree	*exec;
	t_exec	*node;

	node = init_exec();
	exec = parse_cmd_and_redirs(tokens, node);
	node->argv = ft_realloc(node->argv, sizeof(char *) * (node->argc), \
							sizeof(char *) * (node->argc + 1));
	if (!node->argv)
		return (NULL);
	node->argv[node->argc] = NULL;
	return (exec);
}

t_tree	*parse_redir(t_token **tokens, t_tree *child)
{
	t_tree	*redir;
	t_redir	*last_redir;
	t_etype	type;
	char	*s;

	if (!(*tokens) || !((*tokens)->type & REDIR))
		return (NULL);
	type = (*tokens)->type;
	(*tokens) = (*tokens)->next;
	if (!(*tokens) || !((*tokens)->type & STRING))
		return (NULL);
	s = ft_strndup((*tokens)->location.start, (*tokens)->location.len);
	(*tokens) = (*tokens)->next;
	last_redir = get_last_redir(child);
	if (last_redir)
	{
		redir = redir_node(type, last_redir->child, s);
		last_redir->child = redir;
		redir = child;
	}
	else
		redir = redir_node(type, child, s);
	return (redir);
}

t_tree	*parse_block(t_token **tokens)
{
	t_tree	*block;
	t_tree	*child;

	if (!(*tokens) || (*tokens)->type != LPAR)
		return (NULL);
	(*tokens) = (*tokens)->next;
	child = parse_pipe(tokens);
	if (!child)
		return (NULL);
	if ((*tokens) && ((*tokens)->type & CTRL))
	{
		child = parse_sequence_tail(tokens, child);
		if (!child)
			return (NULL);
	}
	if (!(*tokens) || (*tokens)->type != RPAR)
		return (NULL);
	(*tokens) = (*tokens)->next;
	block = block_node(child);
	while ((*tokens) && (*tokens)->type & REDIR)
		block = parse_redir(tokens, block);
	return (block);
}

t_tree	*parse_pipe(t_token **tokens)
{
	t_pipe	*pipe;
	t_tree	*exec;

	pipe = (t_pipe *)pipe_node();
	if (!pipe)
		return (NULL);
	exec = NULL;
	while ((*tokens))
	{
		if ((*tokens)->type == LPAR)
			exec = parse_block(tokens);
		else
			exec = parse_exec(tokens);
		if (!exec)
			return (NULL);
		pipe = (t_pipe *)pipe_add_node((t_tree *)pipe, exec);
		if ((*tokens) && (*tokens)->type == PIPE)
			(*tokens) = (*tokens)->next;
		else
			break ;
	}
	if (pipe->nb_pipes <= 1)
		return (ft_free((void *)pipe, PARSER), exec);
	return ((t_tree *)pipe);
}

t_tree	*parse_sequence(t_token **tokens)
{
	t_tree	*left;

	left = parse_pipe(tokens);
	if (!left)
		return (NULL);
	return (parse_sequence_tail(tokens, left));
}
