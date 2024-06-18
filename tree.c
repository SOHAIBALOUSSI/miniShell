#include "minishell.h"

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
