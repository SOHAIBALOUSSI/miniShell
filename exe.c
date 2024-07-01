#include "../minishell.h"

static int is_builtin(char *cmd)
{
    return (ft_strcmp(cmd , "cd") == 0 || ft_strcmp(cmd , "echo") == 0
        || ft_strcmp(cmd, "env") == 0 || ft_strcmp(cmd, "pwd") == 0
        || ft_strcmp(cmd, "export") == 0 || ft_strcmp(cmd, "unset") == 0
        || ft_strcmp(cmd, "exit") == 0);
}

static char *get_cmd_path(char *cmd)
{
    char *path;
    char *tmp;
    char **paths;
    int i;

    path = get_value("PATH");
    paths = ft_split(path, ':');
    i = 0;
    if (access(cmd, F_OK | X_OK) == 0)
        return (cmd);
    while (paths[i])
    {
        tmp = ft_strjoin(paths[i], "/");
        tmp = ft_strjoin(tmp, cmd);
        if (access(tmp, F_OK | X_OK) == 0)
            return (tmp);
        i++;
    }
    pop_error("Command not found\n");
    return (NULL);
}

int execute_builtin(t_tree *root)
{
    char **argv;

    argv = root->argv;
    if (strcmp(argv[0], "cd") == 0) 
        return (builtin_cd(argv + 1));
    else if (strcmp(argv[0], "echo") == 0) 
        return (builtin_echo(argv + 1));
    else if (strcmp(argv[0], "env") == 0)
    {
        builtin_env();
        return (0);
    }
    else if (strcmp(argv[0], "pwd") == 0) 
        return (builtin_pwd());
    else if (strcmp(argv[0], "export") == 0)
    {
        builtin_export(argv + 1);
        return (0);
    }
    else if (strcmp(argv[0], "unset") == 0) 
        return (builtin_unset(argv + 1));
    else if (strcmp(argv[0], "exit") == 0)
    {
        builtin_exit(argv + 1);
        return (0); 
    }
    return (-1);
}



int execute_cmd(t_tree *root)
{
    char *cmd_path;
    pid_t pid;
    int status;

    // expander(root);
    cmd_path = NULL;
    if (root->argc && is_builtin(root->argv[0]))
        return (execute_builtin(root));
    // expansion of the command
    if (root->argc)
        cmd_path = get_cmd_path(root->argv[0]);
    pid = fork();
    if (pid == 0)
    {
        if (root->redir_list)
            handle_redirections(root->redir_list);
        if (cmd_path && execve(cmd_path, root->argv, __environ) == -1) // environ should be replaced with our env list
            exit(EXIT_FAILURE);
        else if (!cmd_path)
            exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        pop_error("Fork failed\n");
        return (-1);
    }
    else
    {
        waitpid(pid, &status, 0);
        g_shell.exit_status = WEXITSTATUS(status);
    }
    return (g_shell.exit_status);
}

int execute_pipeline(t_tree **pipeline)
{
    int saved_output;
    int saved_input;
    int result;

    if (!pipeline || !(*pipeline))
        return (-1);
    saved_output = dup(STDOUT_FILENO);
    saved_input = dup(STDIN_FILENO);
    result = actual_pipeline(pipeline, g_shell.pipe_count);
    dup2(saved_input, STDIN_FILENO);
    close(saved_input);
    dup2(saved_output, STDOUT_FILENO);
    close(saved_output);
    return (result);   
}

void execute_ast(t_tree *root)
{
    // traverse the ast and execute based on the node type
    // if the node is an Operator, execute the left and right nodes
    // the right node is a command node or pipe_line node
    // if the node is a command node, expend and execute the command
    // if (root->type == _PIPE)
    //     execute_pipeline(root->pipe_line);
    if (root->type == _CMD)
        execute_cmd(root);
}#include "../minishell.h"

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
            handle_here_doc(redir_list);
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
}#include "../minishell.h"

int    actual_pipeline(t_tree **pipeline, int pc)
{
    pid_t   pid;
    int     status;
    int     fd[pc - 1][2];
    int     i;

    i = 0;
    while (i < pc - 1)
    {
        if (pipe(fd[i]) < 0)
        {
            pop_error("Pipe creation failed\n");
            return (-1);
        }
        i++;
    }
    i = 0;
    while (i < pc)
    {
        pid = fork();
        if (pid < 0)
        {
            pop_error("Fork failed\n");
            return (-1);
        }
        else if (pid == 0)
        {
            if (i > 0)
                dup2(fd[i - 1][0], STDIN_FILENO);
            if (i < pc - 1)
                dup2(fd[i][1], STDOUT_FILENO);
            int j = 0;
            while (j < pc - 1)
            {
                close(fd[j][0]);
                close(fd[j][1]);
                j++;
            }
            if (pipeline[i]->redir_list)
                handle_redirections(pipeline[i]->redir_list);
            status = execute_cmd(pipeline[i]);
            exit(status);
        }
        else
        {
            if (i > 0)
            {
                close(fd[i - 1][0]);
                close(fd[i - 1][1]);
            }
            waitpid(pid, &status, 0);
            g_shell.exit_status = WEXITSTATUS(status);
        }
        i++;
    }
    g_shell.pipe_count = 0;
    return (g_shell.exit_status);
}
