#include "minishell.h"


void	catch_syntax_errors(t_token	*token_lst)
{
	t_token *tmp;
	
	tmp = token_lst;
	if (g_shell.single_quote_count % 2 != 0 || g_shell.double_quote_count % 2 != 0)
		return (pop_error("Minishell: syntax error (unclosed quotes)\n"));
	else if (g_shell.closed_paren_count != g_shell.open_paren_count)
		return (pop_error("Minishell: syntax error (unclosed parenthesis)\n"));
	
	// A Command can't start with these .
	if (tmp->type == _PIPE || tmp->type == _OR || tmp->type == _AND) 
		return (pop_error("Minishell: syntax error \n"));
	while (tmp->next)
	{
		// Check for LOGICAL OPERATIONS
		if ((tmp->type == _PIPE || tmp->type == _AND || tmp->type == _OR) 
			&& (tmp->next->type == _PIPE || tmp->next->type == _AND || tmp->next->type == _OR))
			return (pop_error("Minishell: syntax error in operations\n"));

		// Check for REDIRECTION ..

		// MORE CHECKS IDK ...

		tmp = tmp->next;	
	}
}

