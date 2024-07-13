#include "minishell.h"

int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r');
}

int	is_op(char c, char c1)
{
	if (c == '&' && c1 == '&')
		return (1);
	return (c == '|' || c == '>' || c == '<' || c == '(' 
			|| c == ')' || c == '*' || c == '$');
}
int	is_redirection(e_tok type)
{
	return (type == _APPEND || type == _HEREDOC
		|| type == _RED_IN || type == _RED_OUT);
}

int	is_pipe_or_and(e_tok type)
{
	return (type == _PIPE || type == _OR || type == _AND);
}

int	is_word(e_tok type)
{
	return (type == _WORD || type == _ENV
		|| type == _WILDCARD || type == _QUOTE);
}
