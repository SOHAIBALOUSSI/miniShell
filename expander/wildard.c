#include "../minishell.h"

int is_match(char *pattern, char *str)
{
    if (*pattern == '\0' && *str == '\0')
        return (1);
    if (*pattern == '*')
        return (is_match(pattern + 1, str) || (str != '\0' && is_match(pattern, str + 1)));
    return (0);
}

