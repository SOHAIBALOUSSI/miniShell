/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sait-alo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 11:52:35 by sait-alo          #+#    #+#             */
/*   Updated: 2023/11/27 11:52:55 by sait-alo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static int	int_len(long n)
{
	int	count;

	count = 0;
	if (n == 0)
		count++;
	if (n < 0)
	{
		count++;
		n = -n;
	}
	while (n != 0)
	{
		n = n / 10;
		count++;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	long	number;
	int		len;
	char	*res;
	int		i;

	number = n;
	len = int_len(number);
	res = malloc(len + 1 * sizeof(char));
	if (!res)
		return (NULL);
	res[0] = '0';
	if (number < 0)
		number = -number;
	i = len - 1;
	while (number != 0)
	{
		res[i] = ((number % 10) + '0');
		number = number / 10;
		i--;
	}
	if (n < 0)
		res[0] = '-';
	res[len] = 0;
	return (res);
}
