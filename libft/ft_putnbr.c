/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/17 22:30:05 by sait-alo          #+#    #+#             */
/*   Updated: 2024/07/19 17:08:01 by msaadidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putnbr(long n)
{
	int	printed;

	printed = 0;
	if (n < 0)
	{
		printed += ft_putchar('-');
		printed += ft_putnbr(-n);
	}
	else if (n > 9)
	{
		printed += ft_putnbr(n / 10);
		printed += ft_putchar(n % 10 + '0');
	}
	else
		printed += ft_putchar(n + '0');
	return (printed);
}
