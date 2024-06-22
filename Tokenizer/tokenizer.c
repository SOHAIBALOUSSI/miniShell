#include "../minishell.h"

t_token	*tokenizer(char *input)
{
	t_token	*head;
	char	*start;

	head = NULL;
	start = input;
	while (*input)
	{
		if (*input && is_space(*input))
			input++;
		else if (*input && is_op(*input, *(input + 1)))
			input += add_op_token(&head, *input, *(input + 1), input);
		else if (*input && *input == SQ || *input == DQ)
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
	return (head);
}
