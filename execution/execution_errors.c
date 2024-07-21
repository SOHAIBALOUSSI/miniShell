/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_errors.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 18:02:28 by msaadidi          #+#    #+#             */
/*   Updated: 2024/07/20 16:11:20 by msaadidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <errno.h>

void	perror_file(char *file_name)
{
	ft_putstr_fd("Minishell: ", 2);
	if (file_name)
		perror(file_name);
	else
		ft_putendl_fd(strerror(ENOENT), 2);
}

int	perror_ambiguous(char *file_name)
{
	ft_putstr_fd("Minishell: ", 2);
	ft_putstr_fd(file_name, 2);
	ft_putendl_fd(": ambiguous redirect", 2);
	return (EXIT_FAILURE);
}

void	print_error(char *cmd, char *str)
{
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(str, 2);
}

int	minishell_error(char *cmd)
{
	if (!ft_strcmp(cmd, "minishell") || !*cmd)
	{
		print_error(cmd, "command not found");
		mshell()->exit_status = 127;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

char	*check_validity(char *cmd)
{
	if (is_directory(cmd))
	{
		mshell()->exit_status = 126;
		return (print_error(cmd, "Is a directory"), NULL);
	}
	if (!access(cmd, F_OK | X_OK))
		return (cmd);
	mshell()->exit_status = 127;
	return (perror(cmd), NULL);
}
