/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 15:32:22 by sait-alo          #+#    #+#             */
/*   Updated: 2024/07/19 17:10:18 by msaadidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	update_env_var_value(t_env *var, char *new_value, bool has_add_sign)
{
	char	*tmp;

	if (has_add_sign)
	{
		tmp = ft_strjoin(var->value, new_value);
		m_free(var->value);
		var->value = tmp;
	}
	else
	{
		m_free(var->value);
		var->value = ft_strdup(new_value);
	}
}

static char	**sort_array(char **arr)
{
	int		i;
	char	*tmp;

	i = 0;
	while (arr[i])
	{
		if (arr[i + 1] && ft_strcmp(arr[i], arr[i + 1]) >= 0)
		{
			tmp = arr[i];
			arr[i] = arr[i + 1];
			arr[i + 1] = tmp;
			i = 0;
		}
		else
			i++;
	}
	return (arr);
}

char	**lst_to_arr(t_env **env_list)
{
	int		i;
	t_env	*ptr;
	char	**arr;

	i = 0;
	arr = m_alloc((lst_size(env_list) + 1) * sizeof(char *), ALLOC);
	ptr = *env_list;
	while (ptr)
	{
		if (ft_strcmp(ptr->key, "_") == 0)
		{
			ptr = ptr->next;
			continue ;
		}
		arr[i] = ptr->key;
		i++;
		ptr = ptr->next;
	}
	arr[i] = NULL;
	return (arr);
}

static void	put_sorted_env(void)
{
	int		i;
	char	*put;
	char	**env_array;

	i = 0;
	env_array = lst_to_arr(mshell()->env_list);
	env_array = sort_array(env_array);
	while (env_array[i])
	{
		printf("declare -x ");
		put = get_var_value(env_array[i]);
		if (put)
		{
			printf("%s", env_array[i]);
			printf("=\"");
			printf("%s\"\n", put);
		}
		else
			printf("%s\n", env_array[i]);
		i++;
	}
}

void	builtin_export(char **args)
{
	t_env	*var;
	t_env	*existing_var;

	if (!*args)
		return (put_sorted_env());
	while (*args)
	{
		if (is_valid_key(*args))
		{
			var = create_env(*args);
			existing_var = find_env_var(var->key, *mshell()->env_list);
			if (existing_var)
			{
				if (has_add_sign(*args))
					update_env_var_value(existing_var, var->value, true);
				else if (has_only_equal_sign(*args))
					update_env_var_value(existing_var, var->value, false);
			}
			else
				append_env(mshell()->env_list, var);
		}
		else
			built_ins_err(*args);
		args++;
	}
}
