#include "minishell.h"

int	is_redirection(e_tok type)
{
	return (type == _APPEND || type == _HEREDOC || type == _RED_IN || type == _RED_OUT);
}

int	is_logical_op(e_tok	type)
{
	return (type == _PIPE || type == _OR || type == _AND);
}

void	syntax_err(t_token *current)
{
	char *err;
	if (is_redirection(current->type))
		err = ft_strdup("newline");
	else
		err = ft_substr(current->location.location, 0, current->location.length);
	ft_putstr_fd(RED, 2);
	ft_putstr_fd("Minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd(err, 2);
	ft_putstr_fd("\'\n", 2);
	ft_putstr_fd(RESTORE, 2);
	g_shell = (t_minishell){0};
}
/* '  "  *  $  WORD  (  ) >  >>  <  <<   |  ||  && */
/*  ARC lfila7a */
void	catch_syntax_errors(t_token	*token_lst)
{
	t_token *current;
	
	current = token_lst;
	if (g_shell.single_quote_count % 2 != 0 || g_shell.double_quote_count % 2 != 0)
		return (pop_error("Minishell: syntax error (unclosed quotes)\n"));
	else if (g_shell.closed_paren_count != g_shell.open_paren_count)
		return (pop_error("Minishell: syntax error (unclosed parenthesis)\n"));
	
	// A Command can't start with these .
	while (current)
	{
		// Check for LOGICAL OPERATIONS
		if ((is_logical_op(current->type)) && (!current->prev || !current->next || is_logical_op(current->prev->type) || is_redirection(current->prev->type)) )
			return (syntax_err(current));
		if (is_redirection(current->type) && !current->next)
			return (syntax_err(current));
		// if (current->type == _PAREN_OPEN && current->next->type)
		// if ((is_redirection(current->type)) && )
		// Check for REDIRECTION ..

		// MORE CHECKS IDK ...

		current = current->next;	
	}
}

