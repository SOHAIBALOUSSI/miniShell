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

int	main(int ac, char **av, char **env)
{
	int	i = 0;

	g_shell.env_list = get_env_list(env);
	// search_and_change(&g_shell.env_list, "PWD", ft_strdup("batman"));
	char *type[] = {"m=1", "z=2", NULL};
	export(type);
	t_env	*tmp;
	tmp = g_shell.env_list;
	while (tmp)	
	{
		if (tmp == NULL)
			break;
		printf("%s=%s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
	printf("%s", RED);
	export(NULL);

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
