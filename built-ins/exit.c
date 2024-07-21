/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 15:32:22 by sait-alo          #+#    #+#             */
/*   Updated: 2024/07/19 17:08:01 by msaadidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_valid_number(char *str)
{
	int	i;

	i = 0;
	while (str[i] && is_space(str[i]))
		i++;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static int	check_overflow(char *str)
{
	int		i;
	size_t	num;

	i = 0;
	num = 0;
	while (str[i] && is_space(str[i]))
		i++;
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		if ((num > LLONG_MAX / 10) || (num == LLONG_MAX / 10
				&& (str[i] - '0') > LLONG_MAX % 10))
			return (1);
		num = num * 10 + (str[i] - '0');
		i++;
	}
	return (0);
}

static long long	ft_atoll(char *str)
{
	long long	result;
	int			sign;
	int			i;

	result = 0;
	sign = 1;
	i = 0;
	while (str[i] && is_space(str[i]))
		i++;
	if (str[i] == '-')
		sign = -1;
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

static void	print_exit_error(char *arg)
{
	ft_putstr_fd("Minishell: exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putendl_fd(": numeric argument required", 2);
}

int	builtin_exit(char **args)
{
	long long	exit_status;

	if (!mshell()->pipe_count)
		ft_putendl_fd("exit", 2);
	if (!args || !args[0])
		exit_clean(mshell()->exit_status);
	if (!is_valid_number(args[0]) || check_overflow(args[0]))
	{
		print_exit_error(args[0]);
		exit_clean(2);
	}
	exit_status = ft_atoll(args[0]);
	if (args[1])
	{
		ft_putendl_fd("Minishell: exit: too many arguments", 2);
		return (1);
	}
	exit_clean((int)exit_status);
	return (0);
}
