#include "minishell.h"
#include "libs/libft/libft.h"
#include <sys/types.h>
#include <sys/wait.h>

t_minishell	*mshell(void)
{
	static t_minishell	shell;
	return (&shell);
}
void	read_cmd(void);
void	handle_signals(void);

void	handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handle_signals(void)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTERM, SIG_IGN);
	signal(SIGINT, handler);
}

void print_ast(t_tree *root) {
	if (!root)
		exit(1);

	switch (root->type) {
		case _PIPE:
			printf("PIPE:\n");
			for (int i = 0; root->pipe_line[i]; i++) {
				printf("\tCommand %d:\n", i + 1);
				print_ast(root->pipe_line[i]);
			}
			break;
		case _AND:
			printf("AND:\n");
			printf("\tLeft:\n");
			print_ast(root->left);
			printf("\tRight:\n");
			print_ast(root->right);
			break;
		case _OR:
			printf("OR:\n");
			printf("\tLeft:\n");
			print_ast(root->left);
			printf("\tRight:\n");
			print_ast(root->right);
			break;
		case _CMD:
			printf("CMD: ");
			for (int i = 0; root->argv[i]; i++)
				printf("%s ", root->argv[i]);
			printf("\n");
			if (root->redir_list) {
				printf("\tRedirections:\n");
				t_redir *redir = root->redir_list;
				while (redir) {
					printf("\t\t");
					if (redir->type == _RED_IN)
						printf("< %s\n", redir->file_name);
					else if (redir->type == _RED_OUT)
						printf("> %s\n", redir->file_name);
					else if (redir->type == _APPEND)
						printf(">> %s\n", redir->file_name);
					else if (redir->type == _HEREDOC)
						printf("<< %s\n", redir->file_name);
					redir = redir->next;
				}
			}
			break;
		case _SUBSHELL:
			printf("SUBSHELL:\n");
			print_ast(root->subtree);
			if (root->redir_list) {
				printf("\tRedirections:\n");
				t_redir *redir = root->redir_list;
				while (redir) {
					printf("\t\t");
					if (redir->type == _RED_IN)
						printf("< %s\n", redir->file_name);
					else if (redir->type == _RED_OUT)
						printf("> %s\n", redir->file_name);
					else if (redir->type == _APPEND)
						printf(">> %s\n", redir->file_name);
					else if (redir->type == _HEREDOC)
						printf("<< %s\n", redir->file_name);
					redir = redir->next;
				}
			}
			break;
		default:
			printf("Unknown node type\n");
			break;
	}
}

int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r');
}

int     check_spaces(char *line)
{
	int i = 0;
	while (line[i])
	{
		if (!ft_isspace(line[i]))
			return (1);
		i++;
	}
	return (0);
}
void	read_cmd(void)
{
	char	*line;
	t_token	*token_lst;
	t_token *tmp;
	t_tree	*root;

	// char *type[] = {"_WORD", "_QUOTE", "_OR", "PIPE", "AND", "APPEND", "RED_OUT", \
	// 	"RED_IN", "HEREDOC", "PARENT_OPEN", "PARENT_CLISED", "WILDCARD", "ENV", "CMD", "SUBSHELL", "ARG", "SPACE"};
	line = readline(SHELL_PROMPT);
	if (!line)
		return (printf("exit"), exit(-1));
	if (line[0] && check_spaces(line))
		add_history(line);
	token_lst = tokenizer(line);
	if (token_lst)
	{
		if (!catch_syntax_errors(token_lst))
		{
			root = parse_cmd_line(&token_lst);
			if (!root)
				return ;
			if (!execute_ast(root))
				return ;

		}
	}
	// free(line);
}

int	main(int ac, char **av, char **env)
{
	(void)av;
	if (ac != 1)
		return (EXIT_FAILURE);
	if (!isatty(STDIN_FILENO))
		return (printf("Minishell: not a tty\n"), EXIT_FAILURE);
	mshell()->env_list = get_env_list(env);
	handle_signals();
	while (true)
	{
		read_cmd();
		// m_alloc(0, FREE);
	}
	// rl_clear_history();
	return (EXIT_SUCCESS);
}
