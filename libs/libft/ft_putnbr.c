/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: m3ayz00 <m3ayz00@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/17 22:30:05 by sait-alo          #+#    #+#             */
/*   Updated: 2024/07/14 18:53:49 by m3ayz00          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


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
