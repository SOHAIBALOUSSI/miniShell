#include "../minishell.h"

// void    close_redirections(t_redir *redir_list)
// {
//     t_redir *current;

//     current = redir_list;
//     while (current)
//     {
//         if (current->fds[0] > 0)
//             close(current->fds[0]);
//         if (current->fds[1] > 0)
//             close(current->fds[1]);
//         current = current->next;
//     }
// }

void handle_redirections(t_redir *redir_list)
{
    t_redir *current;

    current = redir_list;
    while (current)
    {
        if (current->type == _RED_IN)
        {
            current->fds[0] = open(current->file_name, O_RDONLY);
            if (current->fds[0] < 0)
            {
                perror(current->file_name);
                exit(EXIT_FAILURE);
            }
            current->original_in = dup(STDIN_FILENO);
            dup2(current->fds[0], STDIN_FILENO);
            close(current->fds[0]);
        }
        else if (current->type == _RED_OUT)
        {
            current->fds[1] = open(current->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (current->fds[1] < 0)
            {
                perror(current->file_name);
                exit(EXIT_FAILURE);
            }
            current->original_out = dup(STDOUT_FILENO);
            dup2(current->fds[1], STDOUT_FILENO);
            close(current->fds[1]);
        }
        else if (current->type == _APPEND)
        {
            current->fds[1] = open(current->file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (current->fds[1] < 0)
            {
                perror(current->file_name);
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
                perror(current->file_name);
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
