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

char    *check_command(char *cmd)
{
    if (is_directory(cmd))
    {
        mshell()->exit_status = 126;
        return (print_error(cmd, "Is a directory"), NULL);
    }
    if (!access(cmd, F_OK | X_OK))
        return (cmd);
    mshell()->exit_status = 127;
    return (perror(cmd), NULL);
}

int minishell_error(char *cmd)
{
    if (!ft_strcmp(cmd, "minishell") || !*cmd)
    {
        print_error(cmd, "command not found");
        mshell()->exit_status = 127;
        return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}

static char *get_cmd_path(char *cmd) 
{
	char 		*path;
	char 		*tmp;
	char 		**paths;
	int			i;

    paths = NULL;
    if (minishell_error(cmd))
        return (NULL);
	path = get_value("PATH");
	if (path)
    {
        if (ft_strchr(cmd, '/'))
            return (check_command(cmd));
		else
		{
			if (is_directory(cmd))
            {
                mshell()->exit_status = 127;
                return (print_error(cmd, "command not found"), NULL);
            }
		}
        paths = ft_split(path, ":");
        i = 0;
        while (paths && paths[i])
        {
            tmp = ft_strjoin(paths[i], "/");
            tmp = ft_strjoin(tmp, cmd);
            if (!access(tmp, F_OK | X_OK))
                return (tmp);
            i++;
        }
        if (!access(cmd, F_OK | X_OK))
            return (cmd);
    }
    else
        return (check_command(cmd));
	print_error(cmd, "command not found");
    mshell()->exit_status = 127;
	return (NULL);
}


int execute_builtin(t_tree *root)
{
	char **argv;
	int ret;

    ret = -1;
    argv = root->argv;
    if (root->redir_list)
        handle_redirections2(root->redir_list, &ret);
    if (ret == EXIT_FAILURE)
        return (ret);
    if (ft_strcmp(argv[0], "cd") == 0)
        ret = builtin_cd(argv + 1);
    else if (ft_strcmp(argv[0], "echo") == 0)
        ret = builtin_echo(argv + 1);
    else if (ft_strcmp(argv[0], "env") == 0)
        ret = builtin_env();
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
        ret = builtin_exit(argv + 1);
    restore_redirections(root->redir_list);
    return (ret);
}

char *get_last_arg(char **args)
{
	int i;

	i = 0;
	while (args[i + 1])
		i++;
	return (args[i]);
}

void    set$_(char *key, char *value)
{
	t_env *env;

	env = find_env_var(key, *mshell()->env_list);
	if (env)
		env->value = ft_strdup(value);
	else
		append_env(mshell()->env_list, create_env(ft_strjoin(key, ft_strjoin("=", value))));
}

void    actual_command(t_tree *root, char *cmd_path)
{
    if (cmd_path && execve(cmd_path, root->argv, get_current_env_array()) == -1)
        exit(EXIT_FAILURE);
    else
        exit(EXIT_SUCCESS);
}

void	prepare_command(t_tree *root, char **cmd_path)
{
	if (root->redir_list)
		handle_redirections(root->redir_list); 
	if (root->argv)
	{
		(*cmd_path) = get_cmd_path(root->argv[0]);
		if (!(*cmd_path))
			exit(mshell()->exit_status);
		else if (is_directory((*cmd_path)))
		{
			print_error((*cmd_path), "command not found");
			mshell()->exit_status = 127;
			exit(mshell()->exit_status);
		}
	}
    actual_command(root, *cmd_path);
}

int execute_cmd(t_tree *root)
{
	char *cmd_path;
	pid_t pid;
	int status;

    cmd_path = NULL;
    expander(root);
	if (root->argv)
		set$_("_", get_last_arg(root->argv));
    if (root->argv && root->argv[0] && is_builtin(root->argv[0]))
        return (execute_builtin(root));
    pid = fork();
    if (pid == 0)
		prepare_command(root, &cmd_path);
    else if (pid < 0)
        return (pop_error("Fork failed\n"), 1);
    else
    {
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status))
            mshell()->exit_status = 128 + WTERMSIG(status);
        else
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



