/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: m3ayz00 <m3ayz00@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 15:32:22 by sait-alo          #+#    #+#             */
/*   Updated: 2024/07/17 00:21:13 by m3ayz00          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <signal.h>
# include <unistd.h>
# include <fcntl.h>
# include <limits.h>
# include <stddef.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <dirent.h>
# include <linux/limits.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "./libft/libft.h"

# define ALLOC 'A'
# define FREE 'F'
# define SHELL_PROMPT "\e[0;32m[minishell]\e[0;0m$ "
# define SQUOTE '\''
# define DQUOTE '\"'

/*	Errors	*/
# define HEREDOC_MAX_ERROR	"Minishell: maximum here-document count exceeded\n"
# define NOT_TTY			"Minishell: not a tty\n"
# define _SHLVL	    "Minishell:warning: SHLVL (1000) too high\tresetting to 1\n"
# define EXIT_NAR			"exit\nMinishell: exit: numeric argument required"
# define NOT_DIR            "Minishell: cd :Not a directory\n"

/* Enums for tokenizer && parser */
typedef enum t_tok
{
	_WORD,
	_QUOTE,
	_OR,
	_PIPE,
	_AND,
	_APPEND,
	_RED_OUT,
	_RED_IN,
	_HEREDOC,
	_PAREN_OPEN,
	_PAREN_CLOSED,
	_WILDCARD,
	_ENV,
	_CMD,
	_SUBSHELL,
	_ARGUMENT,
	_SPACE,
}	t_tok;

typedef struct s_quote_state
{
	bool	in_squote;
	bool	in_dquote;
	bool	got_expansion;
	int		i;
}			t_quote_state;

typedef struct s_slice
{
	char	*location;
	size_t	length;
}			t_slice;

/* lexer */
typedef struct s_token
{
	struct s_token	*next;
	struct s_token	*prev;
	char			*heredoc_file;
	char			*delimiter;
	t_slice			location;
	t_tok			type;
}		t_token;

/* The Struct used in the garbage collector */
typedef struct s_gc
{
	void		*ptr;
	struct s_gc	*next;
	struct s_gc	*prev;
}				t_gc;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}				t_env;

typedef struct s_minishell
{
	t_gc	*arena;
	t_env	**env_list;
	char	*pwd;
	size_t	single_quote_count;
	size_t	double_quote_count;
	size_t	open_paren_count;
	size_t	closed_paren_count;
	size_t	pipe_count;
	size_t	heredoc_count;
	int		exit_status;
	int		expand_oho;
	bool	hd_interrupt;
	bool	in_exec;
}	t_minishell;

typedef struct s_redir
{
	struct s_redir	*next;
	char			*file_name;
	int				fds[2];
	int				heredoc_fd;
	int				original_out;
	int				original_in;
	t_tok			type;
	bool			is_ambiguous;
}	t_redir;

typedef struct s_tree
{
	struct s_tree	*left;
	struct s_tree	*right;
	struct s_tree	**pipe_line;
	struct s_tree	*subtree;
	t_redir			*redir_list;
	char			**argv;
	char			*value;
	size_t			pipe_count;
	size_t			argc;
	t_tok			type;
}	t_tree;

/*		Singleton pattern	(Global like)	*/
t_minishell	*mshell(void);

/*		Garbage Collector		*/
void		*m_alloc(size_t __size, char todo);
void		*m_realloc(void *ptr, size_t oldsize, size_t newsize);
void		m_free(void *ptr);

/*		Env		*/
void		get_env_list(char **env);
void		search_and_change(t_env *var);
t_env		*create_env(char *env);
bool		is_exist(char *s);
void		append_env(t_env **lst, t_env *new_env);
char		*get_key(char *s);

/*		Tokenizer		*/
t_token		*tokenize_line(char *input);
int			add_op_token(t_token **head, int c1, int c2, char *start);
size_t		add_quott_token(t_token **head, char *start);
size_t		add_word_token(t_token **head, char *start);
t_token		*creatt_token(t_tok type, char *start, size_t length);
void		append_token(t_token **head, t_token *new_token);
void		pop_error(char *error_msg);
t_tok		decode_type(char c1, char c2);
void		refint_tokens(t_token **head);

/*		Type check*/
int			is_space(char c);
int			is_op(char c1, char c2);
int			is_redirection(t_tok type);
int			is_pipe_or_and(t_tok type);
int			is_word(t_tok type);

/*		AST		*/
t_tree		*parse_cmd_line(t_token **tokens);
t_tree		*create_op_node(t_tok type);
t_redir		*create_redir_node(t_token *token, t_tok redir_type,
				char *heredocfilename);
t_tree		*create_cmd_node(void);
t_tree		*create_subshell_node(t_tree *subshell);
t_tree		*create_pipe_node(void);
void		add_cmd_to_pipeline(t_tree *pipe, t_tree *cmd);
void		add_arg_to_cmd(t_tree *cmd, char *location, size_t length);
char		*ft_strndup(char *s1, size_t n);

/*			Error		*/
void		print_error(char *cmd, char *str);
void		exit_clean(int exit_status);
void		perror_file(char *file_name);
int			perror_ambiguous(char *file_name);

/*			Execute pipeline		*/
int			actual_pipeline(t_tree **pipeline, int pc);

/*			Execute Subshell		*/
int			execute_subshell(t_tree *root);

/*			Execute Operator		*/
int			execute_operator(t_tree *operator);

/*		Open files		*/
void		handle_redirections(t_redir *redir_list, int *ext, int is_builtin);
void		restore_redirections(t_redir *redir);

/*		Execution		*/
int			execute_ast(t_tree *root);
int			is_builtin(char *cmd);
int			execute_builtin(t_tree *root);
int			execute_cmd(t_tree *root);
int			execute_pipeline(t_tree **pipeline, int n_cmd);
void		close_pipes(int fd[][2], int n_cmd);
int			count_pipes(t_tree **pipe_line);
int			actual_pipeline(t_tree **pipeline, int n_cmd);
char 		*get_value(char *key);
void		set_dollar_(char *key, char *value);
char		*get_last_arg(char **args);
void		prepare_command(t_tree *root, char **cmd_path);
char		*check_validity(char *cmd);
char		*check_command(char *cmd, char *path);
int			is_directory(const char *path);
char		*get_path(char *cmd, char *env_path);
int			minishell_error(char *cmd);

/*		Expander		*/
void    	expander(t_tree *root);
char		*expand_arg(char *arg, bool *to_split);
void		add_to_new_argv(char *expanded_arg, char ***expanded_argv, bool to_split);
int			read_expand_write(char *file_name);
char		*handle_dollar_sign(char *arg, int *i, char *result);
char		*expand_heredoc(char *heredoc_content);
char		*ft_strjoin_char(char *str, char c);
char		*expand_var(char *var_name);
char		*expand_variable(char *arg, int *i);
bool    	is_expandable(char c);


/*		Wildcard		*/
void		expand_wildard(char ***old_argv);
void    	expand_redir_wildcard(t_redir **redir);
void		realloc_argv(char ***new_argv, int *argc, char *content);
int			init_vars(char ***matched, int *count, DIR **dir);
int			init_vars2(char **matched, int *count, DIR **dir);
void		init_expand_vars(int *i, int *new_argc, char ***new_argv, char ***matched);
int			is_match(char *pattern, char *str);
void		free_strs(char **strs);


/*			Signals			*/
void		handle_signals(void);
void		handler(int sig);
void		process_handler(int sig);
void		handle_process_signals(void);

/* Expor	t */
void		builtin_export(char **args);
bool		is_valid_key(char *s);
t_env		*find_env_var(char *key, t_env *env_list);
size_t		lst_size(t_env **lst);
void		built_ins_err(char *err_key);
bool		has_add_sign(char *s);
bool		has_only_equal_sign(char *s);
char		*get_var_value(char *key);
char		*get_var_key(char *str);
char		**lst_to_arr(t_env **env_list);

/*	ENV 	*/
int			builtin_env(void);
int			builtin_unset(char **args);
int			builtin_pwd(void);
int			builtin_echo(char **args);
int			builtin_cd(char **args);
int			builtin_exit(char **args);

void		exit_clean(int exit_status);
void		built_ins_err(char *err_key);
size_t		lst_size(t_env **lst);

char		**get_current_env_array(void);

/*			Syntax Error		*/
int			catch_syntax_errors(t_token *token_lst);
void		syntax_err(t_token *current);
char		*remove_quotes(char *str);
int			check_quotes_and_parens(void);
int			check_heredoc(t_token *current);

/*			Utils		*/
char		*remove_quotes(char *str);
bool		has_quotes(char *str);

/*			Here-doc	*/
char		*read_heredoc(char *delimiter);

/*			Helpers		*/
int			check_spaces(char *line);
int			is_space(char c);
void		reset_counters(void);
int			count_words(char *str);
char		**mem_free(char **arr, size_t count);
char		*get_env_value(char *env);
char		*get_env_key(char *s);

#endif /* MINISHELL_H */
