#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "./libft/libft.h"
#include "readline/readline.h"
#include <readline/history.h>


typedef struct	s_tree
{
	int data;
	struct s_tree *lisr;
	struct s_tree *limn;
}				t_tree;


#endif /*MINISHELL_H*/