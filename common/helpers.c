/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 15:32:22 by sait-alo          #+#    #+#             */
/*   Updated: 2024/07/19 17:08:01 by msaadidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_spaces(char *line)
{
	int	i;

	i = -1;
	while (line[++i])
	{
		if (!is_space(line[i]))
			return (1);
	}
	return (0);
}

void	handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	if (mshell()->in_exec == 0 && mshell()->pipe_count == 0)
		rl_redisplay();
	mshell()->exit_status = 130;
}

void	handle_signals(void)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTERM, SIG_IGN);
	signal(SIGINT, handler);
}

void	reset_counters(void)
{
	mshell()->closed_paren_count = 0;
	mshell()->open_paren_count = 0;
	mshell()->single_quote_count = 0;
	mshell()->double_quote_count = 0;
	mshell()->pipe_count = 0;
	mshell()->heredoc_count = 0;
	mshell()->expand_oho = 0;
	mshell()->in_exec = 0;
}

void	*m_realloc(void *ptr, size_t oldsize, size_t newsize)
{
	void	*new_ptr;

	new_ptr = NULL;
	new_ptr = m_alloc(newsize, ALLOC);
	if (ptr && new_ptr)
	{
		ft_memcpy(new_ptr, ptr, oldsize);
		m_free(ptr);
		ptr = NULL;
	}
	return (new_ptr);
}
