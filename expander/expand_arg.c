/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_arg.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 15:32:22 by sait-alo          #+#    #+#             */
/*   Updated: 2024/07/17 17:04:56 by msaadidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
