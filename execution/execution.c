#include "../minishell.h"

static int is_builtin(char *cmd)
{
	return (ft_strcmp(cmd , "cd") == 0 || ft_strcmp(cmd , "echo") == 0
		|| ft_strcmp(cmd, "env") == 0 || ft_strcmp(cmd, "pwd") == 0
		|| ft_strcmp(cmd, "export") == 0 || ft_strcmp(cmd, "unset") == 0
		|| ft_strcmp(cmd, "exit") == 0);
}

int is_directory(const char *path)
{
    struct stat path_stat;
    if (stat(path, &path_stat) == 0)
        return (S_ISDIR(path_stat.st_mode));
    return (0); 
}

void print_error(char *cmd, char *str)
{
    ft_putstr_fd(cmd, 2);
    ft_putstr_fd(": ", 2);
    ft_putendl_fd(str, 2);
}


static char *get_cmd_path(char *cmd) 
{
	char 		*path;
	char 		*tmp;
	char 		**paths;
	int			i;

    paths = NULL;
    if (!ft_strcmp(cmd, "minishell") || !*cmd)
    {
        print_error(cmd, "command not found");
        mshell()->exit_status = 127;
        return (NULL);
    }
	path = get_value("PATH");
	if (path)
    {
        paths = ft_split(path, ":");
        i = 0;
        if (ft_strchr(cmd, '/'))
        {
            if (is_directory(cmd))
            {
                mshell()->exit_status = 126;
                return (print_error(cmd, "Is a directory"), NULL);
            }
            else
            {
                if (!access(cmd, F_OK | X_OK))
                    return (cmd);
                mshell()->exit_status = 127;
                return (perror(cmd), NULL);
            }
        }
        if (!access(cmd, F_OK | X_OK))
            return (cmd);
    }
    else
    {
        if (is_directory(cmd))
        {
            mshell()->exit_status = 126;
            return (print_error(cmd, "Is a directory"), NULL);
        }
        else
        {
            if (!access(cmd, F_OK | X_OK))
                return (cmd);
            mshell()->exit_status = 127;
            return (perror(cmd), NULL); 
        }
    }
	while (paths && paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		tmp = ft_strjoin(tmp, cmd);
		if (access(tmp, F_OK | X_OK) == 0)
			return (tmp);
		i++;
	}
	print_error(cmd, "command not found");
    mshell()->exit_status = 127;
	return (NULL);
}


int execute_builtin(t_tree *root)
{
    char **argv;
    int ret;

    ret = EXIT_FAILURE;
    argv = root->argv;
    if (root->redir_list)
        handle_redirections(root->redir_list);
    if (ft_strcmp(argv[0], "cd") == 0)
        ret = builtin_cd(argv + 1);
    else if (ft_strcmp(argv[0], "echo") == 0)
        ret = builtin_echo(argv + 1);
    else if (ft_strcmp(argv[0], "env") == 0)
    {
        builtin_env();
        ret = EXIT_SUCCESS;
    }
    else if (ft_strcmp(argv[0], "pwd") == 0)
        ret = builtin_pwd();
    else if (ft_strcmp(argv[0], "export") == 0)
    {
        builtin_export(argv + 1);
        ret = EXIT_SUCCESS;
    }
    else if (ft_strcmp(argv[0], "unset") == 0)
        ret = builtin_unset(argv + 1);
    else if (ft_strcmp(argv[0], "exit") == 0)
    {
        builtin_exit(argv + 1);
        ret = EXIT_SUCCESS;
    }
    return (ret);
}


    int execute_cmd(t_tree *root)
    {
        char *cmd_path;
        pid_t pid;
        int status;

        cmd_path = NULL;
        expander(root);
        if (root->argv && root->argv[0] && is_builtin(root->argv[0]))
            return (execute_builtin(root));
        if (root->argv)
        {
            if (root->redir_list)
                handle_redirections(root->redir_list); 
            cmd_path = get_cmd_path(root->argv[0]);
            if (!cmd_path)
                return (mshell()->exit_status);
            else if (is_directory(cmd_path))
            {
                print_error(cmd_path, "command not found");
                mshell()->exit_status = 127;
                return (mshell()->exit_status);
            }
        }
        pid = fork();
        if (pid == 0)
        {
            if (root->redir_list)
                handle_redirections(root->redir_list);
            if (cmd_path && execve(cmd_path, root->argv, get_current_env_array()) == -1)
                exit(EXIT_FAILURE);
            else
                exit(EXIT_FAILURE);
        }
        else if (pid < 0)
        {
            pop_error("Fork failed\n");
            return (1);
        }
        else
        {
            waitpid(pid, &status, 0);
            mshell()->exit_status = WEXITSTATUS(status);
        }
        return (mshell()->exit_status);
    }

int count_pipes(t_tree **pipe_line)
{
	int i;

	i = 0;
	while (pipe_line[i])
		i++;
	return (i);
}
int execute_pipeline(t_tree **pipeline, int n_cmd)
{
	int saved_output;
	int saved_input;
	int result;

	if (!pipeline || !(*pipeline))
		return (1);
	saved_output = dup(STDOUT_FILENO);
	saved_input = dup(STDIN_FILENO);
	result = actual_pipeline(pipeline, n_cmd);
	dup2(saved_input, STDIN_FILENO);
	close(saved_input);
	dup2(saved_output, STDOUT_FILENO);
	close(saved_output);
	return (result);   
}


int execute_ast(t_tree *root)
{
    if (mshell()->hd_interrupt)
    {
        mshell()->hd_interrupt = 0;
        return (mshell()->exit_status);
    }
	if (!root)
		return (1);
	else if (root->type == _AND || root->type == _OR)
		mshell()->exit_status = execute_operator(root);
	else if (root->type == _SUBSHELL)
		mshell()->exit_status = execute_subshell(root);
	else if (root->type == _PIPE)
		mshell()->exit_status = execute_pipeline(root->pipe_line, count_pipes(root->pipe_line));
	else if (root->type == _CMD)
		mshell()->exit_status = execute_cmd(root);
	if (root->redir_list)
		restore_redirections(root->redir_list);
	return (mshell()->exit_status);
}
