/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_base16.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sait-alo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/12 20:19:36 by sait-alo          #+#    #+#             */
/*   Updated: 2023/11/12 20:19:41 by sait-alo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_base16(unsigned long nb, char *base)
{
	int	printed;

	printed = 0;
	if (nb >= 16)
	{
		printed += ft_base16(nb / 16, base);
		printed += ft_base16(nb % 16, base);
	}
	else
		printed += ft_putchar(base[nb]);
	return (printed);
}
