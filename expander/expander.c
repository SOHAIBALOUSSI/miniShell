
#include "../minishell.h"

void	add_to_new_argv(char *expanded_arg, char ***expanded_argv, bool to_split);

static char	*expand_var(char *var_name)
{
	char	*value;

	value = get_value(var_name);
	if (!value)
		return (NULL);
	return (ft_strdup(value));
}

static char	*extract_var_name(char *str)
{
	int	len;

	len = 0;
	while (str[len] && (ft_isalnum(str[len]) || str[len] == '_'))
		len++;
	return (ft_strndup(str, len));
}

static char	*ft_strjoin_char(char *str, char c)
{
	char	*result;
	int		len;
	int		i;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	result = m_alloc(sizeof(char) * (len + 2), ALLOC);
	if (!result)
		return (NULL);
	i = -1;
	while (str[++i])
		result[i] = str[i];
	result[i] = c;
	result[i + 1] = '\0';
	m_free(str);
	return (result);
}

static char	*handle_dollar_sign(char *arg, int *i, char *result, int in_dquote)
{
	char	*var_name;
	char	*var_value;

	(*i)++;
	if (!arg[*i])
		result = ft_strjoin_char(result, '$');
	if (arg[*i] == '?')
	{
		var_value = ft_itoa(mshell()->exit_status);
		result = ft_strjoin(result, var_value);
		m_free(var_value);
		return (result);
	}
	else if (ft_isalnum(arg[*i]) || arg[*i] == '_')
	{
		var_name = extract_var_name(&arg[*i]);
		var_value = expand_var(var_name);
		result = ft_strjoin(result, var_value);
		*i += ft_strlen(var_name) - 1;
		m_free(var_name);
		if (var_value)
			m_free(var_value);
	}
	return (result);
}

static char	*expand_arg(char *arg, bool *to_split)
{
	int		i;
	int		in_squote;
	int		in_dquote;
	char	*result;

	i = 0;
	in_squote = 0;
	in_dquote = 0;
	result = ft_strdup("");
	while (arg[i])
	{
		if (arg[i] == SQUOTE && !in_dquote)
			in_squote = !in_squote;
		else if (arg[i] == DQUOTE && !in_squote)
		{
			*to_split = false;
			in_dquote = !in_dquote;
		}
		else if (arg[i] == '$' && !in_squote && arg[i + 1] && (ft_isalnum(arg[i + 1]) || arg[i + 1] == '?' || arg[i + 1] == '_'))
			result = handle_dollar_sign(arg, &i, result, in_dquote);
		else
			result = ft_strjoin_char(result, arg[i]);
		i++;
	}
	return (result);
}

static void	split_and_add_to_new_argv(char *expanded_arg, char ***expanded_argv)
{
	int		i;
	char	**split;

	split = ft_split(expanded_arg, " \n\t\v\t\r");
	i = 0;
	while (split[i])
	{
		add_to_new_argv(split[i], expanded_argv, false);
		i++;
	}
	m_free(split);
}

char **add_to_argv(char *expanded_arg, char ***expanded_argv)
{
	int	i;
	char **new_argv;

	i = 0;
	while ((*expanded_argv)[i])
		i++;
	new_argv = m_alloc(sizeof(char *) * (i + 2), ALLOC);
	i = 0;
	while ((*expanded_argv)[i])
	{
		new_argv[i] = (*expanded_argv)[i];
		i++;
	}
	new_argv[i] = expanded_arg;
	new_argv[i + 1] = NULL;
	m_free(*expanded_argv);
	return (new_argv);
}

void	add_to_new_argv(char *expanded_arg, char ***expanded_argv, bool to_split)
{
	int		i;
	char	**new_argv;

	if (to_split == true)
		split_and_add_to_new_argv(expanded_arg, expanded_argv);
	else
		*expanded_argv = add_to_argv(expanded_arg, expanded_argv);
}

void	expand_argv(t_tree *node)
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
		add_to_new_argv(expanded_arg, &expanded_argv, to_split);
		i++;
	}
	m_free(node->argv);
	node->argv = expanded_argv;
}

char	*expand_heredoc_line(char *heredoc_line)
{
	char *result;
	int		i;

	i = 0;
	result = ft_strdup("");
	while (heredoc_line[i])
	{
		if (heredoc_line[i] == '$' && heredoc_line[i + 1] && (ft_isalnum(heredoc_line[i + 1]) || heredoc_line[i + 1] == '?'))
			result = handle_dollar_sign(heredoc_line, &i, result, false);
		else
			result = ft_strjoin_char(result, heredoc_line[i]);
		i++;
	}
	return (result);
}

char	*expand_heredoc(char *heredoc_content)
{
	char	*result;
	char	*line;
	char	**content;
	int		i;

	if (mshell()->expand_oho == 1)
		return (heredoc_content);
	i = 0;
	result = ft_strdup("");
	content = ft_split(heredoc_content, "\n");
	while (content[i])
	{
		line = expand_heredoc_line(content[i]);
		result = ft_strjoin(result, line);
		result = ft_strjoin_char(result, '\n');
		m_free(line);
		i++;
	}
	return (result);
}

int	read_expand_write(char *file_name)
{
	char	*line;
	char	*heredoc;
	int		fd;
	char	*expandheredoc;

	fd = open(file_name, O_RDONLY);
	if (fd < 0)
		return (pop_error("open() failed in read_expand_write\n"), -1);
	heredoc = ft_strdup("");
	line = get_next_line(fd);
	if (!line)
		return (-1);
	while (line > 0)
	{
		heredoc = ft_strjoin(heredoc, line);
		m_free(line);
		line = get_next_line(fd);
	}
	expandheredoc = expand_heredoc(heredoc);
	close(fd);
	fd = open(file_name, O_WRONLY | O_TRUNC);
	if (fd < 0)
		return (-1);
	return (write(fd, expandheredoc, ft_strlen(expandheredoc)), close(fd), 0);
}

int	expand_redirection(t_redir *redir_list)
{
	t_redir	*redir;

	redir = redir_list;
	while (redir)
	{
		if (redir->type == _HEREDOC)
		{
			if (read_expand_write(redir->file_name) == -1)
				return (-1);
		}
		else
		if (redir->file_name)
		{
			redir->file_name = expand_arg(redir->file_name, false);
		}
		redir = redir->next;
	}
	return (0);
}

void	expander(t_tree *root)
{
	if (root->argv)
	{
		expand_argv(root);
		expand_wildard(&root->argv);
	}
	if (root->redir_list)
		expand_redirection(root->redir_list);
}