#include "minishell.h"
#include <asm-generic/signal-defs.h>

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
	signal(SIGQUIT, SIG_IGN); // ignore CTRL + '\'
	signal(SIGTERM, SIG_IGN); // ignore SIGTERM same as BASH 
	signal(SIGINT, handler);
}

void	read_cmd(void)
{
	char	*line;
	t_token	*token_lst;
	t_token *tmp;
	t_token	*api;
	// t_tree	*tree;

	char *type[] = {"SPACE", "WORD", "OR", "PIPE", "AND", \
	"REDIRECT", "GREAT", "LESS", "HEREDOC", "PAREN_OPEN", "PAREN_CLOSED", "WILDCARD", \
	"DOUBLE_Q", "Q_CONTENT", "SINGLE_Q", "$ENV", "BAD"};

	line = readline(SHELL_PROMPT);
	if (!line)
		return (printf("exit"), m_alloc(0, FREE), exit(-1));

	add_history(line);
	token_lst = tokenizer(line);
	catch_syntax_errors(token_lst);

	// simplify_tokens(&token_lst);
	// parser(token_lst, tree);


	tmp = token_lst;
	while (tmp != NULL)
	{
		printf("TYPE = [%s] - LENGHT = [%zu]\n", type[tmp->type], tmp->location.length);
		tmp = tmp->next;
	}
	free(line);
}

// void print_env(t_env *head)
// {
// 	t_env *temp;

// 	temp = head;
// 	while (temp)
// 	{
// 		printf("%s\n", (char *)temp->content);
// 		temp = temp->next;
// 	}
// }

// t_list * get_env(char**env)
// {
// 	int i;
// 	t_list *env_list;

// 	env_list = NULL;
// 	i = 0;
// 	while (env[i])
// 	{
// 		ft_lstadd_back(&env_list, ft_lstnew((char *)env[i]));
// 		i++;
// 	}
// 	return (env_list);
// }

// void ft_split_node(t_list *head)
// {
// 	while (head)
// 	{
// 		head->key = ft_substr()
// 		head->value = ft_substr()
// 	}
// 	// x0 -> export var1+=ls
// 	// head search (head +=)
// 	// else if (ft_strcmp(key , ))
// }
// void ft_search(t_list *head, char *target)
// {
// 	// head => PAGER=LS
// 	// key = PAGER ----> SUBSTR(HEAD, 0, FT_FOUND(=));
// 	//value = LS ----> SUBSTR(HEAD, FT_FOUND(=) + 1, FT_STRLEN)HEAD);
// }
int main(int ac, char **av, char **env)
{

	g_shell.env_list = get_env(g_shell.env);
	print_env(g_shell.env_list);
	exit(0);
	handle_signals();
	while (1)
	{

		read_cmd();
		m_alloc(0, FREE);	
	}
	rl_clear_history();
	return (0);
}
