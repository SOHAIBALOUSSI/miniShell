/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sait-alo <sait-alo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 17:02:29 by msaadidi          #+#    #+#             */
/*   Updated: 2024/07/18 13:16:59 by sait-alo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	process_handler(int sig)
{
	(void)sig;
	exit(130);
}

void	handle_process_signals(void)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTERM, SIG_IGN);
	signal(SIGINT, process_handler);
}

void	sig_quit(int sig)
{
	(void)sig;
}

void	phandler(int sig)
{
	(void)sig;
	rl_on_new_line();
	rl_replace_line("", 0);
	if (mshell()->in_exec == 0 && mshell()->pipe_count == 0)
		rl_redisplay();
	mshell()->exit_status = 130;
}

void	handle_p_signals(void)
{
	signal(SIGQUIT, sig_quit);
	signal(SIGTERM, SIG_DFL);
	signal(SIGINT, phandler);
}
