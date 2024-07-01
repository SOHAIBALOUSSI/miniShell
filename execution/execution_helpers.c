#include "../minishell.h"

void    handle_redirections(t_redir *redir_list)
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
                pop_error("Open failed for input redirection\n");
                exit(EXIT_FAILURE);
            }
            dup2(current->fds[0], STDIN_FILENO);
            close(current->fds[0]);
        }
        else if (current->type == _RED_OUT)
        {
            current->fds[1] = open(current->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (current->fds[1] < 0)
            {
                pop_error("Open failed for output redirection\n");
                exit(EXIT_FAILURE);
            }
            dup2(current->fds[1], STDOUT_FILENO);
            close(current->fds[1]);
        }
        else if (current->type == _APPEND)
        {
            current->fds[1] = open(current->file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (current->fds[1] < 0)
            {
                perror("Open failed for append redirection\n");
                exit(EXIT_FAILURE);
            }
            dup2(current->fds[1], STDOUT_FILENO);
            close(current->fds[1]);
        }
        else if (current->type == _HEREDOC)
        {
            // dir chi 7araka hna
            int fd = open(current->file_name, O_RDONLY);
            if (fd < 0)
            {
                pop_error("Open failed for heredoc\n");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        current = current->next;
    }
}

void    here_doc(t_redir *redir_list)
{
    char *line;

    while (1)
    {
        line = readline("> ");
        if (ft_strcmp(redir_list->file_name, line) == 0)
            break;
        write(redir_list->fds[1], line, ft_strlen(line));
        write(redir_list->fds[1], "\n", 1);
        free(line);
    }
}

void    handle_here_doc(t_redir *redir_list)
{
    pid_t pid;

    if (pipe(redir_list->fds) == -1)
    {
        pop_error("Pipe failed\n");
        exit(EXIT_FAILURE);
    }

        close(redir_list->fds[1]);
        waitpid(pid, NULL, 0);
        dup2(redir_list->fds[0], STDIN_FILENO);
        close(redir_list->fds[0]);
}