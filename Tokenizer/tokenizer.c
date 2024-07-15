#include "../minishell.h"

void	add_space_token(t_token **head, char **start)
{
	t_token *token;

	while (is_space(**start))
		(*start)++;
	token = create_token(_SPACE, *start, 1);
	append_token(head, token);
}
t_token	*tokenizer(char *input)
{
	t_token	*head;
	char	*start;

	head = NULL;
	start = input;
	while (*input)
	{
		if (*input && is_space(*input))
			add_space_token(&head, &input);
		else if (*input && is_op(*input, *(input + 1)))
			input += add_op_token(&head, *input, *(input + 1), input);
		else if (*input && *input == SQUOTE || *input == DQUOTE)
		{
			start = input;
			input += add_quote_token(&head, input);
		}
		else
		{
			start = input;
			input += add_word_token(&head, start);
		}
	}
	return (refine_tokens(&head), head);
}
