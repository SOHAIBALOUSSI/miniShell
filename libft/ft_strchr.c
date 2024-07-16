/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: m3ayz00 <m3ayz00@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 11:09:14 by sait-alo          #+#    #+#             */
/*   Updated: 2024/07/14 19:07:53 by m3ayz00          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	int		i;
	char	char_c;

	i = 0;
	char_c = (char)c;
	while (s && s[i])
	{
		if (s[i] == char_c)
			return ((char *) &s[i + 1]);
		i++;
	}
	if (char_c == '\0')
		return ((char *) &s[i]);
	return (NULL);
}
