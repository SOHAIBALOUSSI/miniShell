#include "../minishell.h"

int	add_op_token(t_token **head, int c1, int c2, char *start)
{
	e_tok	type;
	size_t	length;

	type = decode_type(c1, c2);
	g_shell.open_paren_count += (type == _PAREN_OPEN) * 1;
	g_shell.closed_paren_count += (type == _PAREN_CLOSED) * 1;
	length = (type == _HEREDOC || type == _AND ||
			type == _OR || type == _APPEND) * 1 + 1;
	append_token(head, create_token(type, start, length));
	return (length);
}
size_t	add_quote_token(t_token **head, char *start)
{
	char	*p;
	size_t	length;

	p = start;
	length = 1;
	g_shell.single_quote_count += (*p == SQUOTE) * 1;
	g_shell.double_quote_count += (*p == DQUOTE) * 1;
	p++;
	while (*p && (*p != *start))
	{
		p++;
		length++;
	}
	if (*p == SQUOTE || *p == DQUOTE)
	{
		g_shell.single_quote_count += (*p == SQUOTE) * 1;
		g_shell.double_quote_count += (*p == DQUOTE) * 1;
		length++;
		p++;
	}
	append_token(head, create_token(_QUOTE, start, length));
	return (length);
}

size_t	add_word_token(t_token **head, char *start)
{
	char	*p;
	size_t	length;
	
	p = start;
	length = 0;
	while (*p && (!is_space(*p) && !is_op(*p, *(p + 1)))
        && *p!= SQUOTE && *p!= DQUOTE)
	{
		p++;
		length++;
	}
	append_token(head, create_token(_WORD, start, length));
	return (length);
}

t_token	*create_token(e_tok type, char *start, size_t length)
{
	t_token	*new;

	new = m_alloc(sizeof(t_token), ALLOC);
	new->type = type;
	new->location = (t_slice){start, length};
	new->prev = NULL;
	new->next = NULL;
	return (new);
}

void	append_token(t_token **head, t_token *token)
{
	t_token	*last;

	if (!*head)
	{
		*head = token;
		return;
	}
	last = *head;
	while (last->next)
		last = last->next;
	last->next = token;
	token->prev = last;
}