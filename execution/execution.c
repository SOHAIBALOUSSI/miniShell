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

    if (is_builtin(root->argv[0]))
        return (execute_builtin(root));
    // expansion of the command
    cmd_path = get_cmd_path(root->argv[0]);
    pid = fork();
    if (pid == 0)
    {
        if (root->redir_list)
            handle_redirections(root->redir_list);
        if (execve(cmd_path, root->argv, __environ) == -1) // environ should be replaced with our env list
        {
            pop_error("command not found\n");
            exit(EXIT_FAILURE);
        }
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
    if (root->type == _PIPE)
        execute_pipeline(root->pipe_line);
    else if (root->type == _CMD)
        execute_cmd(root);
}