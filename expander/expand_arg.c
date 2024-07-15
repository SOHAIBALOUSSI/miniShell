#include "../minishell.h"

static	char	*handle_exit_status(char *result)
{
	char	*exit_status;

	exit_status = ft_itoa(mshell()->exit_status);
	result = ft_strjoin(result, exit_status);
	m_free(exit_status);
	return (result);
}

char	*handle_dollar_sign(char *arg, int *i, char *result)
{
	char	*var_name;
	char	*var_value;

	(*i)++;
    if (arg[*i] == '?')
        return (handle_exit_status(result));
    var_name = get_var_key(&arg[*i]);
    var_value = get_var_value(var_name);
    result = ft_strjoin(result, var_value);
    *i += ft_strlen(var_name) - 1;
    m_free(var_name);
    m_free(var_value);
	return (result);
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
		if (arg[i] == '=' && arg[i + 1] && arg[i + 1] == '$')
			*to_split = false;
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
