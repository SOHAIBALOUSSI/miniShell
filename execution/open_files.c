/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sait-alo  <sait-alo@student.1337.ma >      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 19:53:11 by sait-alo          #+#    #+#             */
/*   Updated: 2024/07/14 19:02:25 by sait-alo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void    perror_file(char *file_name)
{
    ft_putstr_fd("Minishell: ", 2);
    perror(file_name);
}  

int    perror_ambiguous(char *file_name)
{
    ft_putstr_fd("Minishell: ", 2);
    ft_putstr_fd(file_name, 2);
    ft_putendl_fd(": ambiguous redirect", 2);
    return (EXIT_FAILURE);
}

int handle_input_redirection(t_redir *current)
{
    if (current->is_ambiguous)
        return (perror_ambiguous(current->file_name));
    current->fds[0] = open(current->file_name, O_RDONLY);
    if (current->fds[0] < 0)
        return (perror_file(current->file_name), 1);
    current->original_in = dup(STDIN_FILENO);
    if (!current->next)
        dup2(current->fds[0], STDIN_FILENO);
    close(current->fds[0]);
    return (0);
}
int handle_output_redirection(t_redir *current)
{
    int flags;

    if (current->is_ambiguous)
        return (perror_ambiguous(current->file_name));
    flags = O_WRONLY | O_CREAT;
    if (current->type == _RED_OUT)
        flags |= O_TRUNC;
    else
        flags |= O_APPEND;
    current->fds[1] = open(current->file_name, flags, 0644);
    if (current->fds[1] < 0)
        return (perror_file(current->file_name), 1);
    current->original_out = dup(STDOUT_FILENO);
    if (!current->next)
        dup2(current->fds[1], STDOUT_FILENO);
    close(current->fds[1]);
    return (0);
}

int handle_heredoc(t_redir *current)
{
    int fd;

    fd = open(current->file_name, O_RDONLY);
    if (fd < 0)
        return (perror_file(current->file_name), 1);
    current->original_in = dup(STDIN_FILENO);
    dup2(fd, STDIN_FILENO);
    close(fd);
    return (0);
}
void handle_redirections2(t_redir *redir_list, int *exit)
{
    t_redir *current;
    int errors = 0;

    current = redir_list;
    while (current)
    {
        if (current->type == _RED_IN)
        {
            if (handle_input_redirection(current))
            {
                errors++;
                break ;
            }
        }
        else if (current->type == _RED_OUT || current->type == _APPEND)
        {
            if (handle_output_redirection(current))
            {
                errors++;
                break ;
            }
        }
        else if (current->type == _HEREDOC)
        {
            if (handle_heredoc(current))
            {
                errors++;
                break ;
            }
        }
        current = current->next;
    }
    if (errors > 0)
        *exit = EXIT_FAILURE;
    else
        *exit = EXIT_SUCCESS;
}

void handle_redirections(t_redir *redir_list)
{
    t_redir *current;

    current = redir_list;
    while (current)
    {
        if (current->type == _RED_IN)
        {
            if (current->is_ambiguous)
            {
                perror_ambiguous(current->file_name);
                exit(EXIT_FAILURE);
            }
            current->fds[0] = open(current->file_name, O_RDONLY);
            if (current->fds[0] < 0)
            {
                perror_file(current->file_name);
                exit(EXIT_FAILURE);
            }
            current->original_in = dup(STDIN_FILENO);
            dup2(current->fds[0], STDIN_FILENO);
            close(current->fds[0]);
        }
        else if (current->type == _RED_OUT)
        {
            if (current->is_ambiguous)
            {
                perror_ambiguous(current->file_name);
                exit(EXIT_FAILURE);
            }
            current->fds[1] = open(current->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (current->fds[1] < 0)
            {
                perror_file(current->file_name);
                exit(EXIT_FAILURE);
            }
            current->original_out = dup(STDOUT_FILENO);
            dup2(current->fds[1], STDOUT_FILENO);
            close(current->fds[1]);
        }
        else if (current->type == _APPEND)
        {
            if (current->is_ambiguous)
            {
                perror_ambiguous(current->file_name);
                exit(EXIT_FAILURE);
            }
            current->fds[1] = open(current->file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (current->fds[1] < 0)
            {
                perror_file(current->file_name);
                exit(EXIT_FAILURE);
            }
            current->original_out = dup(STDOUT_FILENO);
            dup2(current->fds[1], STDOUT_FILENO);
            close(current->fds[1]);
        }
        else if (current->type == _HEREDOC)
        {
            int fd = open(current->file_name, O_RDONLY);
            if (fd < 0)
            {
                perror_file(current->file_name);
                exit(EXIT_FAILURE);
            }
            current->original_in = dup(STDIN_FILENO);
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        current = current->next;
    }
}

void restore_redirections(t_redir *redir)
{
    t_redir *current;
    
    current = redir;
    while (current)
    {
        if (current->original_in != -1)
        {
            dup2(current->original_in, STDIN_FILENO);
            close(current->original_in);
        }
        if (current->original_out != -1)
        {
            dup2(current->original_out, STDOUT_FILENO);
            close(current->original_out);
        }
        current = current->next;
    }
}
