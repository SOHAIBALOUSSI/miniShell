#include "../minishell.h"

bool    is_expandable(char c)
{
    return (c && (ft_isalnum(c) || c == '?' || c == '_'));
}
int	count_words(char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str && str[i])
	{
		while (str[i] && is_space(str[i]))
			i++;
		if (str[i] && !is_space(str[i]))
			count++;
		while (str[i] && !is_space(str[i]))
			i++;
	}
	return (count);
}

char	*get_var_key(char *str)
{
	int	len;

	len = 0;
	while (str[len] && (ft_isalnum(str[len]) || str[len] == '_'))
		len++;
	return (ft_strndup(str, len));
}

char	*get_var_value(char *key)
{
	char	*value;

	value = get_value(key);
	if (!value)
		return (NULL);
	return (ft_strdup(value));
}

char	*ft_strjoin_char(char *str, char c)
{
	char	*result;
	int		len;
	int		i;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	result = m_alloc(sizeof(char) * (len + 2), ALLOC);
	if (!result)
		return (NULL);
	i = -1;
	while (str[++i])
		result[i] = str[i];
	result[i] = c;
	result[i + 1] = '\0';
	m_free(str);
	return (result);
}