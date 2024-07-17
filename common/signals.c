/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 17:02:29 by msaadidi          #+#    #+#             */
/*   Updated: 2024/07/17 17:02:41 by msaadidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	sig_quit(int sig)
{
	(void)sig;
	ft_putstr_fd("Quit (core dumped)\n", STDERR_FILENO);
}

void	handle_p_signals(void)
{
	signal(SIGQUIT, sig_quit);
	signal(SIGTERM, SIG_IGN);
	signal(SIGINT, handler);
}
