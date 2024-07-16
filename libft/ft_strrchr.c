/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sait-alo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 09:35:59 by sait-alo          #+#    #+#             */
/*   Updated: 2023/11/07 09:57:26 by sait-alo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int		i;
	char	char_c;

	char_c = (char )c;
	i = ft_strlen(s);
	while (i >= 0)
	{
		if (s[i] == char_c)
			return ((char *)&s[i]);
		i--;
	}
	return (NULL);
}
