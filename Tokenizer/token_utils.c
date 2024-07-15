#include "../minishell.h"

void	pop_error(char *error_msg)
{
	ft_putstr_fd(error_msg, 2);
}

e_tok	decode_type(char c1, char c2)
{
	e_tok	type;

	type = (c1 == '(') * _PAREN_OPEN + (c1 == ')') * _PAREN_CLOSED
		+ (c1 == '$') * _ENV + (c1 == '*') * _WILDCARD
		+ (c1 == '|' && c2 != '|') * _PIPE
		+ (c1 == '|' && c2 == '|') * _OR
		+ (c1 == '<' && c2 != '<') * _RED_IN
		+ (c1 == '<' && c2 == '<') * _HEREDOC
		+ (c1 == '>' && c2 != '>') * _RED_OUT
		+ (c1 == '>' && c2 == '>') * _APPEND
		+ (c1 == '&' && c2 == '&') * _AND;
	return (type);
}

static void remove_spaces(t_token **head)
{
    t_token *current;
    t_token *next;

    if (!head || !*head)
        return;
    current = *head;
    while (current && current->type == _SPACE)
    {
        *head = current->next;
        if (*head)
            (*head)->prev = NULL;
        m_free(current);
        current = *head;
    }
    while (current)
    {
        next = current->next;
        while (next && next->type == _SPACE)
        {
            current->next = next->next;
            if (next->next)
                next->next->prev = current;
            m_free(next);
            next = current->next;
        }
        current = next;
    }
}

static void string_concatenation(t_token **head)
{
	t_token *curr;
	t_token *prev;

	curr = *head;
	prev = NULL;
	while (curr)
	{
		if (is_word(curr->type) && prev && is_word(prev->type))
		{
			prev->location.length += curr->location.length;
			prev->next = curr->next;
			m_free(curr);
			curr = prev;
		}
		prev = curr;
		curr = curr->next;
	}
}

void    refine_tokens(t_token **head)
{
	string_concatenation(head);
	remove_spaces(head);
}