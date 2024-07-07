#include "../minishell.h"

static int is_builtin(char *cmd)
{
	return (ft_strcmp(cmd , "cd") == 0 || ft_strcmp(cmd , "echo") == 0
		|| ft_strcmp(cmd, "env") == 0 || ft_strcmp(cmd, "pwd") == 0
		|| ft_strcmp(cmd, "export") == 0 || ft_strcmp(cmd, "unset") == 0
		|| ft_strcmp(cmd, "exit") == 0);
}

static char *get_cmd_path(char *cmd) // hadchi khaso t3awd
{
	struct stat	file;
	char 		*path;
	char 		*tmp;
	char 		**paths;
	int			i;

	stat(cmd, &file);
	path = get_value("PATH");
	if (!path)
		return(pop_error("PATH: unseted\n"), NULL);
	paths = ft_split(path, ":");
	i = 0; 
	if (!access(cmd, F_OK | X_OK))
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
	if (root->redir_list)
		handle_redirections(root->redir_list);
	if (ft_strcmp(argv[0], "cd") == 0) 
		return (builtin_cd(argv + 1));
	else if (ft_strcmp(argv[0], "echo") == 0) 
		return (builtin_echo(argv + 1));
	else if (ft_strcmp(argv[0], "env") == 0)
	{
		builtin_env();
		return (0);
	}
	else if (ft_strcmp(argv[0], "pwd") == 0) 
		return (builtin_pwd());
	else if (ft_strcmp(argv[0], "export") == 0)
	{
		builtin_export(argv + 1);
		return (0);
	}
	else if (ft_strcmp(argv[0], "unset") == 0) 
		return (builtin_unset(argv + 1));
	else if (ft_strcmp(argv[0], "exit") == 0)
	{
		builtin_exit(argv + 1);
		return (0); 
	}
	return (1);
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
		cmd_path = get_cmd_path(root->argv[0]);
		if (!cmd_path)
			return (127);
	}
	pid = fork();
	if (pid == 0)
	{
		if (root->redir_list)
			handle_redirections(root->redir_list);
		if (cmd_path && execve(cmd_path, root->argv, __environ) == -1)
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
	if (!root)
		return (1);
	else if (root->type == _AND || root->type == _OR)
		mshell()->exit_status = execute_operator(root);
	else if (root->type == _SUBSHELL)
		mshell()->exit_status = execute_subshell(root->subtree);
	else if (root->type == _PIPE)
		mshell()->exit_status = execute_pipeline(root->pipe_line, count_pipes(root->pipe_line));
	else if (root->type == _CMD)
		mshell()->exit_status = execute_cmd(root);
	return (mshell()->exit_status);
}