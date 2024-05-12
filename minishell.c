#include "minishell.h"
#include <asm-generic/signal-defs.h>

char *line;

void	read_cmd(void);
void	handle_signals(void);

t_tree	*new_node(int content)
{
	t_tree	*node;

	node = malloc(sizeof(t_tree));
	if (!node)
		return (NULL);
	node->data = content;
	node->limn = NULL;
	node->lisr = NULL;
	return (node);
}

void	print_tree(t_tree *root)
{
	if (!root)
		return ;
	printf("[%d]\n", root->data);
	printf("Lisr -> ");
	print_tree(root->lisr);
	printf("Limn -> ");
	print_tree(root->limn);
	printf("Done!");
}
void	handler(int sig)
{
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
	line = readline("\e[0;32m[minishell]$ \e[0;0m");
	if (!line)
		exit(-1);
	add_history(line);
	printf("%s\n", line);
	free(line);
}
int main()
{
	handle_signals();
	while (1)
	{
		read_cmd();
	}
	rl_clear_history();
	return (0);
}
