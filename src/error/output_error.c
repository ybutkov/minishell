/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 18:52:49 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/24 02:58:26 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "constants.h"
#include "shell.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void	output_error(const char *obj, char *error)
{
	write(STDERR_FILENO, SHELL_ERROR_PREFIX, ft_strlen(SHELL_ERROR_PREFIX));
	if (obj)
	{
		write(STDERR_FILENO, obj, ft_strlen(obj));
		write(STDERR_FILENO, COLON_SPACE, ft_strlen(COLON_SPACE));
	}
	if (error)
		write(STDERR_FILENO, error, ft_strlen(error));
	else
		write(STDERR_FILENO, strerror(errno), ft_strlen(strerror(errno)));
	write(STDERR_FILENO, NEW_LINE, 1);
}

void	output_error_and_exit(const char *obj, char *error, t_shell *shell,
		int exit_error_code)
{
	output_error(obj, error);
	if (shell)
		shell->free(shell);
	exit(exit_error_code);
}
