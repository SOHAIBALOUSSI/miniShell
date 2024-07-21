/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 15:32:22 by sait-alo          #+#    #+#             */
/*   Updated: 2024/07/20 16:15:51 by msaadidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	check_redir(t_redir *redir)
{
	if (redir->file_name)
		return (check_special_chars(redir->file_name));
	else
		return (1);
}

int	check_files(t_redir *redir)
{
	if (!redir->file_name)
		return (0);
	if (count_words(redir->file_name) > 1)
		return (1);
	if (count_words(redir->file_name) == 0)
		return (1);
	return (0);
}

static int	expand_redirection(t_redir *redir_list)
{
	t_redir	*redir;
	bool	to_split;
	int		did_it_exp;

	redir = redir_list;
	to_split = false;
	did_it_exp = 0;
	while (redir)
	{
		if (redir->type == _HEREDOC)
		{
			if (read_expand_write(redir->file_name) == -1)
				return (-1);
		}
		else if (redir->file_name)
		{
			did_it_exp = check_redir(redir);
			redir->file_name = expand_arg(redir->file_name, &to_split);
			if (did_it_exp && check_files(redir))
				redir->is_ambiguous = 1;
		}
		redir = redir->next;
	}
	return (0);
}

static void	expand_argv(t_tree *node)
{
	int		i;
	bool	to_split;
	char	**expanded_argv;
	char	*expanded_arg;

	i = 0;
	to_split = true;
	expanded_argv = m_alloc(sizeof(char *), ALLOC);
	expanded_argv[0] = NULL;
	while (node->argv[i])
	{
		expanded_arg = expand_arg(node->argv[i], &to_split);
		if (expanded_arg)
			add_to_new_argv(expanded_arg, &expanded_argv, to_split);
		i++;
	}
	node->argv = expanded_argv;
}

void	expander(t_tree *root)
{
	if (root->argv)
	{
		expand_wildard(&root->argv);
		expand_argv(root);
		set_dollar_("_", get_last_arg(root->argv));
	}
	if (root->redir_list)
	{
		expand_redir_wildcard(&root->redir_list);
		expand_redirection(root->redir_list);
	}
}
