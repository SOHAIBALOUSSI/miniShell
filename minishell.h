#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "./libs/libft/libft.h"

# define ALLOC 'A'
# define FREE 'F'
# define SHELL_PROMPT "\e[0;32m[minishell]$ \e[0;0m"
# define SQ '\''
# define DQ '\"'
# define PAREN_ERR	"Minishell: syntax error (unclosed parenthesis)"

/*	COLORS*/
# define RED "\e[0;31m"
# define RESTORE "\e[0;m"

/* The Struct used in the Tokenizer */
typedef enum e_tok
{
	_SPACE, // 0
	_WORD, // 1
	_OR, // 2
	_PIPE, // 3
	_AND, // 4
	_APPEND, // 5
	_RED_OUT, // 6
	_RED_IN, // 7
	_HEREDOC, // 8
	_PAREN_OPEN, // 9
	_PAREN_CLOSED, // 10
	_WILDCARD, // 11
	_DOUBLE_Q, // 12
	_Q_CONTENT, // 13
	_SINGLE_Q, // 14
	_$ENV, // 15
	_BAD, // 16
	_ERROR,
}	e_tok;


typedef struct s_slice
{
	char	*location;
	size_t	length;
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

/*
**	Struct for the g_shell global variable,
** 	You can use it in any file, add a member
** 	to the struct and enjoy the hack :D
*/
typedef	struct s_minishell
{
	size_t	single_quote_count;
	size_t	double_quote_count;
	size_t	open_paren_count;
	size_t	closed_paren_count;
	int		exit_status;
}			t_minishell;

extern	t_minishell g_shell;

typedef struct	s_tree
{
	int data;
	struct s_tree *lisr;
	struct s_tree *limn;
}				t_tree;

void	*m_alloc(size_t __size, char todo);
t_token	*tokenizer(char *input);
e_tok	decode_type(char c1, char c2);
void	pop_error(char *error_msg);
void	catch_syntax_errors(t_token	*token_lst);

#endif /*	MINISHELL_H	*/
