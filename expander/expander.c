#include "../minishell.h"

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
	if (arg[*i] == '?' && (!arg[*i + 1] || arg[*i + 1] == ' ' || 
		(in_dquote && arg[*i + 1] == '\"')))
	{
		var_value = ft_itoa(g_shell.exit_status);
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

char	*expand_arg(char *arg)
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
			in_dquote = !in_dquote;
		else if (arg[i] == '$' && !in_squote)
			result = handle_dollar_sign(arg, &i, result, in_dquote);
		else
			result = ft_strjoin_char(result, arg[i]);
		i++;
	}
	return (result);
}

char	**expand_argv(char **argv)
{
	char	**expanded_argv;
	int		i;

	i = 0;
	while (argv[i])
		i++;
	expanded_argv = m_alloc(sizeof(char *) * (i + 1), ALLOC);
	i = 0;
	while (argv[i])
	{
		expanded_argv[i] = expand_arg(argv[i]);
		i++;
	}
	expanded_argv[i] = NULL;
	return (expanded_argv);
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