#include "minishell.h"
#include <asm-generic/signal-defs.h>

char *line;

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
	line = readline("\e[0;32m[minishell]$ \e[0;0m");
	if (!line) // EOF
		exit(0);
	add_history(line);
	char **tokens = history_tokenize(line);
	int i = 0;
	while (tokens[i])
	{
		printf("%s\n", tokens[i]);
		i++;
	}
	
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
