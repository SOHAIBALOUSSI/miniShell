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
#include <dirent.h>
#include <limits.h>
#include <linux/limits.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "./libs/libft/libft.h"

# define ALLOC 'A'
# define FREE 'F'
# define SHELL_PROMPT "\e[0;32m[minishell]\e[0;0m$ "
# define SQUOTE '\''
# define DQUOTE '\"'

/*	Errors	*/
# define HEREDOC_MAX_ERROR	"Minishell: maximum here-document count exceeded\n"
# define NOT_TTY			"Minishell: not a tty\n"
# define HIGH_SHLVL			"Minishell: warning: shell level (1000) too high, resetting to 1\n"
# define EXIT_NAR		"exit\nMinishell: exit: numeric argument required"

/* The Struct used in the Tokenizer */
typedef enum e_tok
{
	_WORD, // 0
	_QUOTE, // 1
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
	_CMD, // 13
	_SUBSHELL, // 14
	_ARGUMENT, // 15
	_SPACE,  //16
}	e_tok;

typedef	struct s_quote_state
{
	bool	in_squote;
	bool	in_dquote;
	bool	got_expansion;
	bool	to_split;
}			t_quote_state;

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
	char			*heredoc_file;
	char 			*delimiter;
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

typedef	struct s_minishell
{
	t_gc	*arena;
	size_t	single_quote_count;
	size_t	double_quote_count;
	size_t	open_paren_count;
	size_t	closed_paren_count;
	size_t	pipe_count;
	size_t	heredoc_count;
	t_env	**env_list;
	char	*pwd;
	int		exit_status;
	int		expand_oho;
	bool	hd_interrupt;
	bool	in_exec;
}			t_minishell;

typedef struct s_redir
{
	e_tok	type;
	int		fds[2];
	int		heredoc_fd;
	int		original_out;
	int		original_in;
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


/*		Globals		*/
t_minishell	*mshell(void);
 
/*		Garbage Collector		*/
void	*m_alloc(size_t __size, char todo);
void	*m_realloc(void *ptr, size_t oldsize, size_t newsize);
void	m_free(void *ptr);

/*		Env		*/
void	get_env_list(char **env);
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
int			catch_syntax_errors(t_token *token_lst);
e_tok		decode_type(char c1, char c2);
void		refine_tokens(t_token **head);

/*		Type check*/
int			is_space(char c);
int			is_op(char c1, char c2);
int			is_redirection(e_tok type);
int			is_pipe_or_and(e_tok type);
int			is_word(e_tok type);

/*		AST		*/
t_tree	*parse_cmd_line(t_token **tokens);
t_tree	*create_op_node(e_tok type);
t_redir	*create_redir_node(t_token *token, e_tok redir_type, char *heredocfilename);
t_tree	*create_cmd_node(void);
t_tree	*create_subshell_node(t_tree *subshell);
t_tree	*create_pipe_node(t_token **tokens);
void	add_cmd_to_pipeline(t_tree *pipe, t_tree *cmd);
void	add_arg_to_cmd(t_tree *cmd, char *location, size_t length);
char	*ft_strndup(char *s1, size_t n);

/*		Error		*/
void print_error(char *cmd, char *str);

/*		Execute pipeline		*/
int    actual_pipeline(t_tree **pipeline, int pc);

/*		Execute Subshell		*/
int execute_subshell(t_tree *root);

/*		Execute Operator		*/
int execute_operator(t_tree *operator);

/*		Open files		*/
void    handle_redirections(t_redir *redir_list);
void handle_redirections2(t_redir *redir_list, int *exit);
void restore_redirections(t_redir *redir);

/*		Execution		*/
int execute_ast(t_tree *root);
int execute_cmd(t_tree *root);

char *get_value(char *key);


/*		Expander		*/
void    expander(t_tree *root);
void	expand_wildard(char ***old_argv);
char	*expand_arg(char *arg, bool *to_split);
void	add_to_new_argv(char *expanded_arg, char ***expanded_argv, bool to_split);
int		read_expand_write(char *file_name);
char	*handle_dollar_sign(char *arg, int *i, char *result);
char	*expand_heredoc(char *heredoc_content);
char	*ft_strjoin_char(char *str, char c);
char	*expand_var(char *var_name);
char	*expand_variable(char *arg, int *i);

void    expand_wildard(char ***old_argv);
void    free_strs(char **strs);
char	**get_matching_files(char *pattern);
int		is_match(char *pattern, char *str);

/*		Signals			*/
void	handle_signals(void);
void	handler(int sig);


/* Export */

bool	is_valid_key(char *s);
t_env	*find_env_var(char *key, t_env *env_list);
size_t	lst_size(t_env **lst);
void	built_ins_err(char *err_key);
bool	has_add_sign(char *s);
bool	has_only_equal_sign(char *s);
char	*get_var_value(char *key);
char	*get_var_key(char *str);

/*	ENV */

void	builtin_export(char **args);
int		builtin_env(void);
int		builtin_unset(char **args);
int		builtin_exit(char **args);
int		builtin_pwd(void);
int		builtin_echo(char **args);
int		builtin_cd(char **args);

char	**get_current_env_array(void);

#endif /*	MINISHELL_H	*/
