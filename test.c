#include <stdio.h>

int is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\v' || c == '\r');
}
int is_op(char c)
{
	return (c == '|' || c == '>' || c == '<' || c == '&' || c == '(' || c == ')');
}

int	count_tokens(char *input)
{

	int counter = 0;
	while (*input)
	{
		if (*input && is_space(*input))
			counter++;
		while (*input && is_space(*input))
			input++;
		if (*input && is_op(*input))
			counter++;
		while (*input && is_op(*input))
			input++;
		if (*input && (!is_space(*input) && !is_op(*input)))
			counter++;
		while (*input && (!is_space(*input) && !is_op(*input)))
			input++;
	}
	return (counter);
}


int main()
{
	printf("num of tokens [%d]\n", count_tokens("ls -la | grep me >> out.c"));
}