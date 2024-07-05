#include "../minishell.h"

int is_match(char *pattern, char *str)
{
    if (*pattern == '\0' && *str == '\0')
        return (1);
    if (*pattern == '*')
        return (is_match(pattern + 1, str) || (*str != '\0' && is_match(pattern, str + 1)));
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
        if (entry->d_name[0] == '.')
            continue;
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

void    free_strs(char **strs)
{
    int i;

    i = 0;
    while (strs[i])
    {
        m_free(strs[i]);
        i++;
    }
    m_free(strs);
}

void    realloc_argv(char ***new_argv, int *argc, char *content)
{
    (*new_argv) = m_realloc((*new_argv), sizeof(char *) * (*argc), sizeof(char *) * (*argc) + 2);
    (*new_argv)[(*argc)] = ft_strdup(content);
    (*argc)++;
    (*new_argv)[(*argc)] = NULL;
}

void    expand_wildard(char ***old_argv)
{
    int i;
    char **matched;
    char **new_argv;
    int j;
    int new_argc;

    i = 0;
    new_argc = 0;
    while ((*old_argv)[i])
    {
        if (ft_strchr((*old_argv)[i], '*') != NULL)
        {
            matched = get_matching_files((*old_argv)[i]);
            if (matched[0])
            {
                j = 0;
                while (matched[j])
                {
                    realloc_argv(&new_argv, &new_argc, matched[j]);
                    j++;
                }
                free_strs(matched);
            }
            else
            {
                free_strs(matched);
                realloc_argv(&new_argv, &new_argc, (*old_argv)[i]);
            }
        }
        else
            realloc_argv(&new_argv, &new_argc, (*old_argv)[i]);
        i++;
    }
    free_strs((*old_argv));
    (*old_argv) = new_argv;
}
