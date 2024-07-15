#include "../minishell.h"


static int	expand_redirection(t_redir *redir_list)
{
	t_redir	*redir;
	bool	to_split;

	redir = redir_list;
	to_split = false;
	while (redir)
	{
		if (redir->type == _HEREDOC)
		{
			if (read_expand_write(redir->file_name) == -1)
				return (-1);
		}
		else if (redir->file_name)
		{
			redir->file_name = expand_arg(redir->file_name, &to_split);
			if (count_words(redir->file_name) == 0 || count_words(redir->file_name) > 1)
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
	// mem_free(node->argv, i);
	// m_free(node->argv);
	node->argv = expanded_argv;
}

void	expander(t_tree *root)
{
	if (root->argv)
	{
		expand_argv(root);
		expand_wildard(&root->argv);
	}
	if (root->redir_list)
	{
		expand_redirection(root->redir_list);
		expnd_redir_wildcard(&root->redir_list);
	}
}