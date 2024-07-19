/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_arg.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sait-alo <sait-alo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 15:32:22 by sait-alo          #+#    #+#             */
/*   Updated: 2024/07/18 15:46:31 by sait-alo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_special_var(char *key)
{
	if ((!ft_strcmp("PWD", key) || !ft_strcmp("OLDPWD", key)))
		return (1);
	return (0);
}

char	*handle_special_var(char *key, char *result, int *i)
{
	if (!ft_strcmp("PWD", key))
	{
		*i += ft_strlen(key) - 1;
		if (!mshell()->pwd)
			return (ft_strjoin(result, ""));
		return (ft_strjoin(result, mshell()->pwd));
	}
	else if (!ft_strcmp("OLDPWD", key))
	{
		*i += ft_strlen(key) - 1;
		if (!mshell()->oldpwd)
			return (ft_strjoin(result, ""));
		return (ft_strjoin(result, mshell()->oldpwd));
	}
	return (NULL);
}

char	*handle_dollar_sign(char *arg, int *i, char *result)
{
	char	*var_name;
	char	*var_value;

	(*i)++;
	if (arg[*i] == '$')
		return (ft_strjoin(result, ft_strdup("")));
	if (arg[*i] == '?')
		return (ft_strjoin(result, ft_itoa(mshell()->exit_status)));
	var_name = get_var_key(&arg[*i]);
	var_value = get_var_value(var_name);
	if (is_special_var(var_name) == 1 && !var_value)
		return (handle_special_var(var_name, result, i));
	result = ft_strjoin(result, var_value);
	*i += ft_strlen(var_name) - 1;
	m_free(var_name);
	m_free(var_value);
	return (result);
}

char	*handle_expansion(char *arg, int *i, char *result, bool *got_expansion)
{
	*got_expansion = 1;
	return (handle_dollar_sign(arg, i, result));
}

char	*expand_arg(char *arg, bool *to_split)
{
	char			*result;
	t_quote_state	qs;

	qs = (t_quote_state){0};
	result = ft_strdup("");
	while (arg[qs.i])
	{
		if (arg[qs.i] == '=' && arg[qs.i + 1] && arg[qs.i + 1] == '$')
			*to_split = false;
		if (arg[qs.i] == SQUOTE || arg[qs.i] == DQUOTE)
			*to_split = false;
		if (arg[qs.i] == SQUOTE && !qs.in_dquote)
			qs.in_squote = !qs.in_squote;
		else if (arg[qs.i] == DQUOTE && !qs.in_squote)
			qs.in_dquote = !qs.in_dquote;
		else if (arg[qs.i] == '$' && is_exp(arg[qs.i + 1]) && !qs.in_squote)
			result = handle_expansion(arg, &qs.i, result, &qs.got_expansion);
		else
			result = ft_strjoin_char(result, arg[qs.i]);
		qs.i++;
	}
	if ((qs.got_expansion == 1) && result[0] == '\0')
		return (m_free(result), NULL);
	return (result);
}
