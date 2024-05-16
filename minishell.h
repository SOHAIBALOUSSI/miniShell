#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
# include <signal.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "./libs/libft/libft.h"

# define ALLOC 'A'
# define FREE 'F'
# define SHELL_PROMPT "\e[0;32m[minishell]$ \e[0;0m"

/* The Struct used in the Tokenizer */
typedef enum e_tok
{
	_SPACE, // 0
	_WORD, // 1
	_OR, // 2
	_PIPE, // 3
	_AMPER, // 4
	_AND, // 5
	_REDIRECT, // 6
	_GREAT, // 7
	_LESS, // 8
	_HEREDOC, // 9
	_PAREN_L, // 10
	_PAREN_R, // 11
	_WILDCARD, // 12
	_DOUBLE_Q, // 13
	_Q_CONTENT, // 14
	_SINGLE_Q, // 15
	_$ENV, // 16
}	e_tok;


typedef struct s_slice
{
	char	*location;
	size_t	lenght;
}			t_slice;

/* lexer */
typedef struct s_token
{
	e_tok			type;
	t_slice			location;
	struct s_token	*next;
	struct s_token	*prev;
}   			t_token;

/* The Struct used in the garbage collector */
typedef struct s_gc
{
	void		*ptr;
	struct s_gc	*next;
}				t_gc;

typedef	struct s_minishell
{
	size_t	quote_count;

}			t_minishell;



typedef struct	s_tree
{
	int data;
	struct s_tree *lisr;
	struct s_tree *limn;
}				t_tree;

void	*m_alloc(size_t __size, char todo);
t_token	*tokenizer(char *input);

#endif /*	MINISHELL_H	*/
