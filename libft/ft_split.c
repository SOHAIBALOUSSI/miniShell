/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/10 21:21:36 by sait-alo          #+#    #+#             */
/*   Updated: 2024/07/19 17:08:01 by msaadidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "../minishell.h"

static size_t	_count_words(const char *s, char *set)
{
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		while (s[i] && ft_strchr(set, s[i]))
			i++;
		if (s[i])
			count++;
		while (s[i] && !ft_strchr(set, s[i]))
			i++;
	}
	return (count);
}

char	**mem_free(char **arr, size_t count)
{
	size_t	i;

	i = 0;
	while (i < count)
	{
		m_free(arr[i]);
		i++;
	}
	m_free(arr);
	return (NULL);
}

static char	*get_next_word(const char *s, char *set, size_t *start)
{
	size_t	end;
	char	*word;

	while (s[*start] && ft_strchr(set, s[*start]))
		(*start)++;
	end = *start;
	while (s[end] && !ft_strchr(set, s[end]))
		end++;
	word = ft_substr(s, *start, end - *start);
	*start = end;
	return (word);
}

char	**ft_split(char const *s, char *set)
{
	char	**result;
	size_t	word_count;
	size_t	i;
	size_t	start;

	if (!s || !set)
		return (NULL);
	word_count = _count_words(s, set);
	result = m_alloc((word_count + 1) * sizeof(char *), ALLOC);
	if (!result)
		return (NULL);
	i = 0;
	start = 0;
	while (i < word_count)
	{
		result[i] = get_next_word(s, set, &start);
		if (!result[i])
			return (mem_free(result, i));
		i++;
	}
	result[word_count] = NULL;
	return (result);
}
