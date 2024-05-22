#include "minishell.h"

void	pop_error(char *error_msg)
{
	ft_putstr_fd(RED, 2);
	ft_putstr_fd(error_msg, 2);
	ft_putstr_fd(RESTORE, 2);
	g_shell = (t_minishell){0};
}

e_tok	decode_type(char c1, char c2)
{
	e_tok	type;

	type = (c1 == '(') * _PAREN_OPEN + (c1 == ')') * _PAREN_CLOSED
		+ (c1 == '$') * _$ENV + (c1 == '*') * _WILDCARD + (c1 == '|' && c2 != '|') * _PIPE
		+ (c1 == '|' && c2 == '|') * _OR + (c1 == '<' && c2 != '<') * _RED_IN
		+ (c1 == '<' && c2 == '<') * _HEREDOC + (c1 == '>' && c2 != '>') * _RED_OUT
		+ (c1 == '>' && c2 == '>') * _APPEND + (c1 == '&' && c2 == '&') * _AND
		+ (c1 == '&' && c2 != '&') * _BAD;
	return (type);
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

int	is_space(char c)
{
	return (c == ' ' || c == '\t');
}

int	is_op(char c, char c1)
{
	if (c == '&' && c1 == '&')
		return (1);
	return (c == '|' || c == '>' || c == '<' || c == '(' 
			|| c == ')' || c == '*' || c == '$');
}

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

size_t	add_word_token(t_token **head, char *start)
{
	char	*p;
	size_t	length;
	
	p = start;
	length = 0;
	while (*p && (!is_space(*p) && !is_op(*p, *(p + 1))) && (*p != '\'' && *p != '"'))
	{
		p++;
		length++;
	}
	append_token(head, create_token(_WORD, start, length));
	return (length);
}

int	add_quote(t_token **head, e_tok type, char *start)
{
	g_shell.single_quote_count += (type == _SINGLE_Q) * 1;
	g_shell.double_quote_count += (type == _DOUBLE_Q) * 1;
	append_token(head, create_token(type, start, 1));
	if (*start)
		return (1);
	return (0);
}

size_t	add_literal(t_token **head, char *line, char type)
{
	char	*p;
	size_t	length;
	
	p = line;
	length = 0;
	while (*p && *p != type)
	{
		p++;
		length++;
	}
	append_token(head, create_token(_Q_CONTENT, line, length));
	return (length);
}

size_t	add_quote_token(t_token **head, char *line)
{
	size_t	size;
	
	size = 0;
	if (*line == SQ)
	{
		line += add_quote(head, _SINGLE_Q, line);
		if (*line && *line != SQ)
			size += add_literal(head, line, SQ);
		line += size;
		if (*line == SQ)
			size += add_quote(head, _SINGLE_Q, line);
	}
	else if (*line == DQ)
	{
		line += add_quote(head, _DOUBLE_Q, line);
		if (*line && *line != DQ)
			size += add_literal(head, line, DQ);
		line += size;
		if (*line == DQ)
			size += add_quote(head, _DOUBLE_Q, line);
	}
	return (size + 1);
}

t_token	*tokenizer(char *input)
{
	char	*start;
	t_token	*head;

	head = NULL;
	start = input;
	while (*input != '\0')
	{
		if (*input && is_space(*input))
			input++;
		else if (*input && is_op(*input, *(input + 1)))
			input += add_op_token(&head, *input, *(input + 1), input);
		else if (*input && (*input == SQ || *input == DQ))
		{
			start = input;
			input += add_quote_token(&head, start);
		}
		else
		{
			start = input;
			input += add_word_token(&head, start);
		}
	}
	return (head);
}
