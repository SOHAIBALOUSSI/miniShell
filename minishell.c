#include "minishell.h"
#include "libs/libft/libft.h"

t_minishell g_shell = {0};

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
        return;

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




void	read_cmd(void)
{
	char	*line;
	t_token	*token_lst;
	t_token *tmp;
	t_token	*api;
	t_tree	*root;

	char *type[] = {"_WORD", "_QUOTE","_OR", "_PIPE", "_AND", "_APPEND", "_RED_OUT", "_RED_IN", \
	 "_HEREDOC", "_PAREN_OPEN", "_PAREN_CLOSED", "_WILDCARD", "$ENV", "_CMD"};

	line = readline(SHELL_PROMPT);
	if (!line)
		return (printf("exit"), exit(-1));

	add_history(line);
	token_lst = tokenizer(line);
	catch_syntax_errors(token_lst);

	root = parser(token_lst);
	print_ast(root);
	// simplify_tokens(&token_lst);
	// tmp = token_lst;
	// while (tmp != NULL)
	// {
	// 	printf("TYPE = [%s] - LENGHT = [%zu]\n", type[tmp->type], tmp->location.length);
	// 	tmp = tmp->next;
	// }
	// free(line);
}

int	main(int ac, char **av, char **env)
{
	g_shell.env_list = get_env_list(env);

	handle_signals();
	while (1)
	{
		read_cmd();
		m_alloc(0, FREE);
	}
	rl_clear_history();
	return (0);
}
