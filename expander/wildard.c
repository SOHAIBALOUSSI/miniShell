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


char **get_matching_files(char *pattern)
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
    closedir(dir);
    if (count == 0)
    {
        matched = malloc(sizeof(char *) * 1);
        matched[0] = NULL;
    }
    return (matched);
}

void    wildard(char **args)
{
    int i;
    char **matched;
    int j;

    while (args[i])
    {
        if (ft_strchr(args[i], '*') != NULL)
        {
            matched = get_matching_files(args[i], args);
            if (matched)
            {
                j = 0;
                while (matched[j])
                {
                    args[i] = m_realloc(args[i], ft_strlen(args[i]), ft_strle(args[i]) + ft_strlen(matched[j]));
                    j++;
                }
                free_matched(matched);
            }
        }
        i++;
    }
}