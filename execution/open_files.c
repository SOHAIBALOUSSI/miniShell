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
                print_error(current->file_name, strerror(errno));
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
                print_error(current->file_name, strerror(errno));
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
                print_error(current->file_name, strerror(errno));
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
                print_error(current->file_name, strerror(errno));
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
        if (redir->original_in != -1)
        {
            dup2(redir->original_in, STDIN_FILENO);
            close(redir->original_in);
        }
        if (redir->original_out != -1)
        {
            dup2(redir->original_out, STDOUT_FILENO);
            close(redir->original_out);
        }
        current = current->next;
    }
}