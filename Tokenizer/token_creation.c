#include "../minishell.h"

int	add_op_token(t_token **head, int c1, int c2, char *start)
{
	e_tok	type;
	size_t	length;
	t_token	*token;

	type = decode_type(c1, c2);
	mshell()->open_paren_count += (type == _PAREN_OPEN) * 1;
	mshell()->closed_paren_count += (type == _PAREN_CLOSED) * 1;
	mshell()->heredoc_count += (type == _HEREDOC) * 1;
	length = (type == _HEREDOC || type == _AND ||
			type == _OR || type == _APPEND) * 1 + 1;
	token = create_token(type, start, length);
	append_token(head, token);
	return (length);
}
size_t	add_quote_token(t_token **head, char *start)
{
	char	*p;
	size_t	length;
	t_token	*token;

	p = start;
	length = 1;
	mshell()->single_quote_count += (*p == SQUOTE) * 1;
	mshell()->double_quote_count += (*p == DQUOTE) * 1;
	p++;
	while (*p && (*p != *start))
	{
		p++;
		length++;
	}
	if (*p == SQUOTE || *p == DQUOTE)
	{
		mshell()->single_quote_count += (*p == SQUOTE) * 1;
		mshell()->double_quote_count += (*p == DQUOTE) * 1;
		length++;
		p++;
	}
	token = create_token(_QUOTE, start, length);
	append_token(head, token);
	return (length);
}

size_t	add_word_token(t_token **head, char *start)
{
	char	*p;
	size_t	length;
	t_token	*token;

	p = start;
	length = 0;
	while (*p && (!is_space(*p) && !is_op(*p, *(p + 1)))
        && *p!= SQUOTE && *p!= DQUOTE)
	{
		p++;
		length++;
	}
	token = create_token(_WORD, start, length);
	append_token(head, token);
	return (length);
}

t_token	*create_token(e_tok type, char *start, size_t length)
{
	t_token	*new;

	new = m_alloc(sizeof(t_token), ALLOC);
	new->type = type;
	new->location = (t_slice){start, length};
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
