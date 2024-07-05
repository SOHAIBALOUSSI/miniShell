
#include "../minishell.h"

void	add_to_new_argv(char *expanded_arg, char ***expanded_argv, bool to_split);

static char	*expand_var(char *var_name)
{
	char	*value;

	value = get_value(var_name);
	if (!value)
		return (ft_strdup(""));
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
	if (arg[*i] == '?')
	{
		var_value = ft_itoa(mshell()->exit_status);
		result = ft_strjoin(result, var_value);
		m_free(var_value);
		(*i)++;
	}
	else if (ft_isalpha(arg[*i]) || arg[*i] == '_')
	{
		var_name = extract_var_name(&arg[*i]);
		var_value = expand_var(var_name);
		result = ft_strjoin(result, var_value);
		*i += ft_strlen(var_name) - 1;
		m_free(var_name);
		m_free(var_value);
	}
	else
		result = ft_strjoin_char(result, '$');
	return (result);
}

static char	*expand_arg(char *arg, bool *to_split)
{
	char	*result;
	int		i;
	int		in_squote;
	int		in_dquote;

	result = ft_strdup("");
	i = 0;
	in_squote = 0;
	in_dquote = 0;
	while (arg[i])
	{
		if (arg[i] == SQUOTE && !in_dquote)
			in_squote = !in_squote;
		else if (arg[i] == DQUOTE && !in_squote)
		{
			*to_split = false;
			in_dquote = !in_dquote;
		}
		else if (arg[i] == '$' && !in_squote)
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

void	add_to_new_argv(char *expanded_arg, char ***expanded_argv, bool to_split)
{
	int		i;
	char	**new_argv;

	if (to_split == true)
		split_and_add_to_new_argv(expanded_arg, expanded_argv);
	else
	{
		i = 0;
		while ((*expanded_argv)[i])
			i++;
		new_argv = m_alloc(sizeof(char *) * (i + 2), ALLOC);
		i = -1;
		while ((*expanded_argv)[++i])
			new_argv[i] = (*expanded_argv)[i];
		new_argv[i] = expanded_arg;
		new_argv[i + 1] = NULL;
		m_free(*expanded_argv);
		*expanded_argv = new_argv;
	}
}

void	expand_argv(t_tree *node)
{
	int		i;
	char	**expanded_argv;
	char	*expanded_arg;
	bool	to_split;

	i = 0;
	expanded_argv = m_alloc(sizeof(char *), ALLOC);
	expanded_argv[0] = NULL;
	to_split = true;
	while (node->argv[i])
	{
		expanded_arg = expand_arg(node->argv[i], &to_split);
		add_to_new_argv(expanded_arg, &expanded_argv, to_split);
		i++;
	}
	m_free(node->argv);
	node->argv = expanded_argv;
}

void	expander(t_tree *root)
{
	if (root->argv)
	{
		expand_argv(root);
		expand_wildard(&root->argv);
	}
	// if (root->redir_list)
	// Handle redirection expansion if needed
	// if (root->redir_list)
	//     expand_redirection(root->redir_list);
}