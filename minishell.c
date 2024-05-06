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

       #include <stdio.h>
       #include "readline/readline.h"
       #include <readline/history.h>

/* binary tree */
int main()
{
	char *line;
	// t_tree *root = new_node(1);
	// t_tree *l1_n1 = new_node(2);
	// t_tree *l1_n2 = new_node(3);
	// t_tree *l2_n1 = new_node(4);
	// t_tree *l2_n2 = new_node(5);
	// t_tree *l2_n3 = new_node(6);
	// t_tree *l2_n4 = new_node(7);


	// root->limn = l1_n1;
	// root->lisr = l1_n2;
	// l1_n1->limn = l2_n1;
	// l1_n1->lisr = l2_n2;
	// l2_n1->limn = l2_n3;
	// l2_n1->lisr = l2_n4;
	// print_tree(root);

	line = readline("minishell");
	printf("%s\n", line);
}