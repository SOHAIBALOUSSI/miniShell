/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putadress.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sait-alo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 18:23:43 by sait-alo          #+#    #+#             */
/*   Updated: 2023/12/18 18:23:45 by sait-alo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putadress(void *arg)
{
	int				printed;
	unsigned long	arg2;
	char			*base;

	if (!arg)
		return (write(1, "(nil)", 5));
	printed = 0;
	arg2 = (unsigned long)arg;
	base = "0123456789abcdef";
	printed += ft_putstr("0x");
	printed += ft_base16(arg2, base);
	return (printed);
}
