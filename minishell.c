#include "minishell.h"
#include <asm-generic/signal-defs.h>


void	read_cmd(void);
void	handle_signals(void);

void	handler(int sig)
{
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handle_signals(void)
{
	signal(SIGQUIT, SIG_IGN); // ignore CTRL + '\'
	signal(SIGTERM, SIG_IGN); // ignore SIGTERM same as BASH 
	signal(SIGINT, handler);
}

void	read_cmd(void)
{
	char	*line;
	t_token	*token_lst;
	t_token *tmp;

	line = readline(SHELL_PROMPT);
	if (!line) // CTRL + D
		return(printf("exit\n"), exit(-1));
	add_history(line);
	token_lst = tokenizer(line);
	
	// tmp = token_lst;
	// while (tmp != NULL)
	// {
	// 	printf("TYPE = [%d] LENGHT = [%zu]\n", tmp->type, tmp->location.lenght);
	// 	tmp = tmp->next;
	// }
	free(line);
}
int main()
{
	handle_signals();
	while (1)
	{
		read_cmd();
	}
	rl_clear_history();
	return (0);
}
