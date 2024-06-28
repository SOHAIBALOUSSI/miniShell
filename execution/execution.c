#include "../minishell.h"
#include <sys/wait.h>

char *get_cmd_path(char *cmd)
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

void execute_cmd(t_tree *root)
{
    char *cmd_path;
    char			**argv;
    // expention of the command
    cmd_path = get_cmd_path(root->argv[0]);
    if (fork() == 0)
    {
        if (execve(cmd_path, root->argv, __environ) == -1)
        {
            exit(1);
        }
    }
    else
    {
        wait(NULL);
    }
    // exit(0);
}

void execute_ast(t_tree *root)
{
    // traverse the ast and execute based on the node type
    // if the node is an Operator, execute the left and right nodes
    // the right node is a command node or pipe_line node
    // if the node is a command node, expend and execute the command
    if (root->type == _CMD)
    {
        execute_cmd(root);
    }
}