#include "../minishell.h"

bool    is_expandable(char c)
{
    return (c && (ft_isalnum(c) || c == '?' || c == '_'));
}

char	*handle_dollar_sign(char *arg, int *i, char *result)
{
	char	*var_name;
	char	*var_value;
	(*i)++;
    if (arg[*i] == '?')
    {
        result = ft_strjoin(result, ft_itoa(mshell()->exit_status));
        return (result);
    }
    var_name = get_var_key(&arg[*i]);
    var_value = get_var_value(var_name);
    result = ft_strjoin(result, var_value);
    *i += ft_strlen(var_name) - 1;
    m_free(var_name);
    m_free(var_value);
	return (result);
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

void	split_and_add_to_new_argv(char *expanded_arg, char ***expanded_argv)
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
	if (to_split == true)
		split_and_add_to_new_argv(expanded_arg, expanded_argv);
	else
		*expanded_argv = add_to_argv(expanded_arg, expanded_argv);
}

char	*handle_expansion(char *arg, int *i, char *result ,bool *got_expansion)
{
	*got_expansion = 1;
	return (handle_dollar_sign(arg, i, result));
}

char	*expand_arg(char *arg, bool *to_split)
{
	int		i;
	char	*result;
	t_quote_state qs;

	i = 0;
	qs = (t_quote_state){0};
	result = ft_strdup("");
	while (arg[i])
	{
		if (arg[i] == SQUOTE || arg[i] == DQUOTE)
			*to_split = false;
		if (arg[i] == SQUOTE && !qs.in_dquote)
			qs.in_squote = !qs.in_squote;
		else if (arg[i] == DQUOTE && !qs.in_squote)
			qs.in_dquote = !qs.in_dquote;
		else if (arg[i] == '$' && is_expandable(arg[i + 1]) && !qs.in_squote)
			result = handle_expansion(arg, &i, result, &qs.got_expansion);
		else
			result = ft_strjoin_char(result, arg[i]);
		i++;
	}
	if ((qs.got_expansion == 1) && result[0] == '\0')
		return (m_free(result), NULL);
	return (result);
}
