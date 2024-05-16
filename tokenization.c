
#include "minishell.h"

t_minishell		g_shell = {0};

e_tok decode_type(char c1, char c2)
{
	if (c1 == '|' && c2 != '|')
		return (_PIPE);
	else if (c1 == ' ')
		return (_SPACE);
	else if (c1 == '|' && c2 == '|')
		return (_OR);
	else if (c1 == '<' && c2 != '<')
		return (_LESS);
	else if (c1 == '<' && c2 == '<')
		return (_HEREDOC);
	else if (c1 == '>' && c2 != '>')
		return (_GREAT);
	else if (c1 == '>' && c2 == '>')
		return (_REDIRECT);
	else if (c1 == '&' && c2!= '&')
		return (_AMPER);
	else if (c1 == '&' && c2 == '&')
		return (_AND);
	else if (c1 == '(')
		return (_PAREN_L);
	else if (c1 == ')')
		return (_PAREN_R);
	else if (c1 == '\"')
		return (_DOUBLE_Q);
	else if (c1 == '\'')
		return (_SINGLE_Q);
	else if (c1 == '$')
		return (_$ENV);
	else if (c1 == '*')
		return (_WILDCARD);
	return (_WORD);
}

t_token	*make_new_node(e_tok type, char *start, size_t length)
{
	t_token *new;

	new = m_alloc(sizeof(t_token), ALLOC);
	new->type = type;
	new->location = (t_slice){start, length};
	new->prev = NULL;
	new->next = NULL;
	return (new);
}


int is_space(char c)
{
	return (c == ' ' || c == '\t');
}
int is_op(char c)
{
	return (c == '|' || c == '>' || c == '<' || c == '&' || c == '(' 
			|| c == ')' || c == '*' || c == '$');
}

int	add_op_token(t_token **head, int c1, int c2, char *start)
{
	e_tok			type;
	t_token			*new;
	t_token			*last;
	size_t			length;

	last = *head;
	type = decode_type(c1, c2);
	length = (1 * (type == _HEREDOC || type == _AND \
		|| type == _OR || type == _REDIRECT)) + 1;
	if (!*head)
	{
		*head = make_new_node(type, start, length);
		return (length);
	}
	new = make_new_node(type, start, length);
	while (last->next)
		last = last->next;
	last->next = new;
	new->prev = last;
	return (length);
}

void	add_word_token(t_token **head, char *start, size_t word$ize)
{
	t_token			*new;
	t_token			*last;

	last = *head;
	if (!*head)
	{
		*head = make_new_node(_WORD, start, word$ize);
		return ;
	}
	new = make_new_node(_WORD, start, word$ize);
	while (last->next)
		last = last->next;
	last->next = new;
	new->prev = last;
}
void	add_quote(t_token **head, e_tok type, char *start)
{
	t_token		*new;
	t_token		*last;
	if (!*head)
	{
		*head = make_new_node(type, start, 1);
		return ;
	}
	new = make_new_node(type, start, 1);
	while (last->next)
		last = last->next;
	last->next = new;
	new->prev = last;
	return ;
}

void	add_quote_content(t_token **head, char *q_content, size_t content$ize)
{
	t_token		*new;
	t_token		*last;
	if (!*head)
	{
		*head = make_new_node(_Q_CONTENT, q_content, content$ize);
		return ;
	}
	new = make_new_node(_Q_CONTENT, q_content, content$ize);
	while (last->next)
		last = last->next;
	last->next = new;
	new->prev = last;
	return ;
}


int	add_quote_token(t_token **head, char *start)
{
	char		*save_;
	size_t		content_size;

	content_size = 0;
	if (*start == '\'')
		add_quote(head, _SINGLE_Q, *start);
	else
		add_quote(head, _DOUBLE_Q, *start);
	start++;
	save_ = start;
	while (*start && *start != '\'' && *start != '\"')
	{
		content_size++;
		*start++;
	}
	add_quote_content(head, save_, content_size);
	if (*start == '\'')
		add_quote(head, _SINGLE_Q, *start);
	else 	
		add_quote(head, _DOUBLE_Q, *start);
	return (content_size);
}

t_token	*tokenizer(char *input)
{
	t_token	*head;
	size_t	word$ize;
	char	*start;

	head = NULL;
	while (*input)
	{
		word$ize = 0;
		if (*input && is_space(*input))
			input += add_op_token(&head, *input, *(input + 1), start);
		while (*input && is_space(*input))
			input++;
		if (*input && (*input == '\'' || *input == '\"'))
			input += add_quote_token(&head, *input, start);
		start = input;
		if (*input && is_op(*input))
			input += add_op_token(&head, *input, *(input + 1), start);
		start = input;
		while (*input && (!is_space(*input) && !is_op(*input)))
		{
			input++;
			word$ize++;
		}
		if (word$ize != 0)
			add_word_token(&head, start, word$ize);
	}
	return (head);
}

