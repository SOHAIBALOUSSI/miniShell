#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include <linux/limits.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "./libs/libft/libft.h"

# define ALLOC 'A'
# define FREE 'F'
# define SHELL_PROMPT "\e[0;32m[minishell]$ \e[0;0m"
# define SQUOTE '\''
# define DQUOTE '\"'

/*	COLORS*/
// # define RED "\e[0;31m"
// # define RESTORE "\e[0;m"

/* The Struct used in the Tokenizer */
typedef enum e_tok
{
	_WORD, // 1
	_QUOTE, // 2
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
	_ENV, // 12
	_CMD, // 14
	_SUBSHELL, // 15
	_ARGUMENT, // 16
	_SPACE, 
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

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}				t_env;

/*
**	Struct for the g_shell global variable,
** 	You can use it in any file, add a member
** 	to the struct and enjoy the hack :D
*/
typedef	struct s_minishell
{
	t_gc	*arena;
	size_t	single_quote_count;
	size_t	double_quote_count;
	size_t	open_paren_count;
	size_t	closed_paren_count;
	size_t	pipe_count;
	t_env	*env_list;
	int		exit_status;
	int		is_add;
	int		is_subshell;
}			t_minishell;

extern	t_minishell g_shell;

typedef struct s_redir
{
	e_tok	type;
	int		fds[2];
	char	*file_name;
	struct s_redir	*next;
}	t_redir;

typedef struct s_tree
{
	e_tok			type;
	size_t			pipe_count;
	struct s_tree	*left;
	struct s_tree	*right;
	struct s_tree	**pipe_line;
	struct s_tree	*subtree;
	t_redir			*redir_list;
	char			**argv;
	size_t			argc;
	char			*value;
}               t_tree;

char	**lst_to_arr(t_env **env_list);

/*		Garbage Collector		*/

void	*m_alloc(size_t __size, char todo);
void	*m_realloc(void *ptr, size_t oldsize, size_t newsize);
void	m_free(void *ptr);

/*		Env		*/
t_env	*get_env_list(char **env);
void	search_and_change(t_env *var);
t_env	*create_env(char *env);
bool	is_exist(char *s);
void	append_env(t_env **lst, t_env *new_env);
char 	*get_key(char *s);

/*		Tokenizer		*/
t_token		*tokenizer(char *input);
int			add_op_token(t_token **head, int c1, int c2, char *start);
size_t		add_quote_token(t_token **head, char *start);
size_t		add_word_token(t_token **head, char *start);
t_token		*create_token(e_tok type, char *start, size_t length);
void		append_token(t_token **head, t_token *new_token);
void		pop_error(char *error_msg);
int		catch_syntax_errors(t_token *token_lst);
e_tok		decode_type(char c1, char c2);
void    	refine_tokens(t_token **head);

/*		Type check*/
int			is_space(char c);
int			is_op(char c1, char c2);
int			is_redirection(e_tok type);
int			is_pipe_or_and(e_tok type);
int			is_word(e_tok type);

/*		AST		*/
t_tree	*parse_cmd_line(t_token **tokens);
t_tree	*create_op_node(e_tok type);
t_redir	*create_redir_node(e_tok type, char *file_name, size_t length);
t_tree	*create_cmd_node(void);
t_tree	*create_subshell_node(t_tree *subshell);
t_tree	*create_pipe_node(t_token **tokens);
void	add_cmd_to_pipeline(t_tree *pipe, t_tree *cmd);
void	add_arg_to_cmd(t_tree *cmd, char *location, size_t length);
char	*ft_strndup(char *s1, size_t n);

/*		Pipeline helpers		*/
int    actual_pipeline(t_tree **pipeline, int pc);


/*		Execution helpers		*/
void    handle_redirections(t_redir *redir_list);
void    here_doc(t_redir *redir_list);
void    handle_here_doc(t_redir *redir_list);

/*		Execution		*/
void execute_ast(t_tree *root);
int execute_cmd(t_tree *root);

/*		Expander		*/
void    expander(t_tree *root);



/* Export */

void	builtin_export(char **args);
bool	is_valid_key(char *s);
char	*get_value(char *key);
t_env	*find_env_var(char *key, t_env *env_list);
size_t	lst_size(t_env **lst);
void	built_ins_err(char *err_key);

/*	ENV */

void	builtin_env(void);
int		builtin_unset(char **args);
void	builtin_exit(char **args);
int		builtin_pwd(void);
int		builtin_echo(char **args);
int		builtin_cd(char **args);

#endif /*	MINISHELL_H	*/
