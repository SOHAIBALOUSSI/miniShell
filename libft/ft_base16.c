/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_base16.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/12 20:19:36 by sait-alo          #+#    #+#             */
/*   Updated: 2024/07/19 17:08:01 by msaadidi         ###   ########.fr       */
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
