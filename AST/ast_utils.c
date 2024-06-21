#include "../minishell.h"

char	*ft_strndup(char *s1, size_t n)
{
	size_t	i;
	char	*new;

	i = 0;
	new = m_alloc(sizeof(char) * (n + 1), ALLOC);
	while (s1[i] && i < n)
	{
		new[i] = s1[i];
		i++;
	}
	new[i] = '\0';
	return (new);
}

void	add_arg_to_cmd(t_tree *cmd, char *location, size_t length)
{
	size_t	argc;

	argc = cmd->argc;
	cmd->argv = m_realloc(cmd->argv, sizeof(char *) * (argc + 1),
            sizeof(char *) * (argc + 2));
	cmd->argv[argc] = ft_strndup(location, length);
	cmd->argv[argc + 1] = NULL;
	cmd->argc++;
}

void    add_cmd_to_pipeline(t_tree *pipe, t_tree *cmd)
{
	pipe->pipe_line = m_realloc(pipe->pipe_line, sizeof(t_tree *) * pipe->pipe_count,
                    sizeof(t_tree *) * (pipe->pipe_count + 1));
	pipe->pipe_line[pipe->pipe_count] = cmd;
	pipe->pipe_count++;
}

