
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

	last = *head;
	if (!*head)
	{
		*head = make_new_node(type, start, 1);
		printf("here\n");
		return ;
	}
	new = make_new_node(type, start, 1);
	while (last->next)
		last = last->next;
	last->next = new;
	new->prev = last;
}

void add_quote_content(t_token **head, char type, char *q_content)
{
	t_token	*new;
	t_token	*last;
	size_t	content_size;
	char	*start;
	
	start = q_content;
    content_size = 0;
    while (*q_content != type && *q_content)
    {
        content_size++;
        q_content++;
    }
    last = *head;
    if (!*head)
    {
        *head = make_new_node(_Q_CONTENT, start, content_size);
        return;
    }
    new = make_new_node(_Q_CONTENT, start, content_size);
    while (last->next)
        last = last->next;
    last->next = new;
    new->prev = last;
}


int add_quote_token(t_token **head, char type, char *start)
{
	// e_tok e_type;

	// e_type = ((type == '\'') * _SINGLE_Q ) + ((type == '\"') * _DOUBLE_Q);
	// add_litteral(head, type, start);
    // add_quote(head, e_type, start);
    // add_quote_content(head, type, start + 1);
    // return (1);

	
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
		start = input;
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

