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
    ERROR, // ??
    _WORD,
    _SPECIAL, // ??
    _OR,
    _PIPE,
    _AMPER,
    _AND,
    _REDIRECT,
    _GREAT,
    _LESS,
    _HEREDOC,
    _PAREN_L,
    _PAREN_R,
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
}   			t_token;

/* The Struct used in the garbage collector */
typedef struct s_gc
{
	void		*ptr;
	struct s_gc	*next;
}				t_gc;



typedef struct	s_tree
{
	int data;
	struct s_tree *lisr;
	struct s_tree *limn;
}				t_tree;

void	*m_alloc(size_t __size, char todo);
t_token	*lexer(char *input);

#endif /*	MINISHELL_H	*/
