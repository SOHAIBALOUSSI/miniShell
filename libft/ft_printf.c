/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sait-alo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/17 22:28:25 by sait-alo          #+#    #+#             */
/*   Updated: 2023/12/17 22:28:28 by sait-alo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static	int	ft_putunsigned(unsigned int n)
{
	int	printed;

	printed = 0;
	if (n > 9)
	{
		printed += ft_putnbr(n / 10);
		printed += ft_putchar(n % 10 + '0');
	}
	else
		printed += ft_putchar(n + '0');
	return (printed);
}

static	int	ft_check_format_and_put(char type, va_list args)
{
	int	printed;

	printed = 0;
	if (type == 'c')
		printed += ft_putchar(va_arg(args, int));
	if (type == 's')
		printed += ft_putstr(va_arg(args, char *));
	if (type == 'd' || type == 'i')
		printed += ft_putnbr(va_arg(args, int));
	if (type == 'u')
		printed += ft_putunsigned(va_arg(args, unsigned int));
	if (type == 'x' || type == 'X')
		printed += ft_puthex(va_arg(args, int), type);
	if (type == 'p')
		printed += ft_putadress(va_arg(args, void *));
	if (type == '%')
		printed += ft_putchar('%');
	return (printed);
}

int	ft_printf(const char *format, ...)
{
	int		i;
	int		printed;
	va_list	args;

	i = 0;
	printed = 0;
	va_start(args, format);
	if (!format)
		return (-1);
	while (format[i] != 0)
	{
		if (format[i] == '%')
		{
			i++;
			printed += ft_check_format_and_put(format[i], args);
		}
		else
			printed += write(1, &format[i], 1);
		i++;
	}
	va_end(args);
	return (printed);
}
