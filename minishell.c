#include "minishell.h"
#include <asm-generic/signal-defs.h>

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
void	handler(int sig, siginfo_t *info, void *context)
{
	(void)context;
		// printf("ctrl + c catched!\n");
	// rl_on_new_line();
	readline(">");	

}

int main()
{
	struct sigaction sa;

	char *line;
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = handler;
	sigaction(SIGINT,&sa, NULL);
	line = readline("\e[0;32m[minishell]>_\e[0;0m");
	while (line)
	{
		add_history(line);
		printf("%s\n", line);
		free(line);
		line = readline("\e[0;32m[minishell]>_\e[0;0m");
	}
	rl_clear_history();
	return (free(line), 0);
}
