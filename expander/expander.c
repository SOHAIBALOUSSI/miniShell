#include "../minishell.h"


void    add_to_new_argv(char *expanded_arg, char ***expanded_argv, bool to_split);

char	*expand_var(char *var_name)
{
	char	*value;

	value = get_value(var_name);
	if (!value)
		return (ft_strdup(""));
	return (ft_strdup(value));
}

char	*extract_var_name(char *str)
{
	int	len;

	len = 0;
	while (str[len] && (ft_isalnum(str[len]) || str[len] == '_'))
		len++;
	return (ft_strndup(str, len));
}


char	*ft_strjoin_char(char *str, char c)
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
	i = 0;
	while (str[i])
	{
		result[i] = str[i];
		i++;
	}
	result[i] = c;
	result[i + 1] = '\0';
	m_free(str);
	return (result);
}
char	*handle_dollar_sign(char *arg, int *i, char *result, int in_dquote)
{
	char	*var_name;
	char	*var_value;

	(*i)++;
	if (arg[*i] == '?') // ( && (!arg[*i + 1] || arg[*i + 1] == ' ' || (in_dquote && arg[*i + 1] == '\"')) || arg[*i] == '?' )
	{
		var_value = ft_itoa(mshell()->exit_status = 2);
		result = ft_strjoin(result, var_value);
		m_free(var_value);
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

char *expand_arg(char *arg, bool *to_split)
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
		if (arg[i] == '\'' && !in_dquote)
			in_squote = !in_squote;
		else if (arg[i] == '\"' && !in_squote)
		{
			*to_split = 0;
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

void	split_and_add_to_new_argv(char *expanded_arg, char ***expanded_argv)
{
	int		i;
	char	**split;

	split = ft_split(expanded_arg, ' ');
	i = 0;
	while (split[i])
	{
		add_to_new_argv(split[i], expanded_argv, 0);
		i++;
	}
	m_free(split);
}

void    add_to_new_argv(char *expanded_arg, char ***expanded_argv, bool to_split)
{
 	int		i;
	char	**new_argv;

	i = 0;
	if (to_split == true)
		split_and_add_to_new_argv(expanded_arg, expanded_argv);
	else
	{
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
		*expanded_argv = new_argv;
	}
}

void    expand_argv(t_tree *node)
{
	int		i;
	char	**expanded_argv;
	char	*expanded_arg;
	bool	to_split;

	i = 0;
	expanded_argv = m_alloc(sizeof(char *) * 1, ALLOC);
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

void    expander(t_tree *root)
{
	if (root->type == _CMD)
		expand_argv(root);
	// else if (root->redir_list)
			   
}
void	free_expanded_argv(char **expanded_argv)
{
	int	i;

	i = 0;
	while (expanded_argv[i])
	{
		m_free(expanded_argv[i]);
		i++;
	}
	m_free(expanded_argv);
}