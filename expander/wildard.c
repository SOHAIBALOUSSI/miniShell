#include "../minishell.h"

int is_match(char *pattern, char *str)
{
    if (*pattern == '\0' && *str == '\0')
        return (1);
    if (*pattern == '*')
        return (is_match(pattern + 1, str) || (str != '\0' && is_match(pattern, str + 1)));
    if (*pattern == *str)
        return (is_match(pattern + 1, str + 1));
    return (0);
}


char **get_matchig_files(char *pattern, char *args[])
{
    char            **matched;
    int             count;
    DIR             *dir;
    struct dirent   *entry;

    matched = NULL;
    count = 0;
    dir = opendir(".");
    if (!dir)
    {
        perror("opendir");
        return (NULL);
    }
    while ((entry = readdir(dir)) != NULL)
    {
        if (is_match(pattern, entry->d_name))
        {
            matched = m_realloc(matched,  sizeof(char *) * (count + 1), sizeof(char *) * (count + 2));
            matched[count] = ft_strdup(entry->d_name);
            count++;
            matched[count] = NULL;
        }
    }
    return (matched);
}

