/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sait-alo <sait-alo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 15:32:22 by sait-alo          #+#    #+#             */
/*   Updated: 2024/07/18 11:50:42 by sait-alo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_error(void)
{
	if (mshell()->pwd == NULL)
	{
		perror("getcwd: ");
		return ;
	}
	printf("%s\n", mshell()->pwd);
}

int	builtin_pwd(void)
{
	char	pwd[PATH_MAX];

	if (!getcwd(pwd, PATH_MAX))
		return (handle_error(), EXIT_SUCCESS);
	printf("%s\n", pwd);
	return (EXIT_SUCCESS);
}
