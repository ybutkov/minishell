/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 18:52:49 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/04 16:35:32 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "constants.h"
#include "shell.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


/**
 * output_error
 * Writes a formatted error message to stderr using minishell conventions.
 *
 * Format:
 *   "minishell: " + [obj + ": "] + (error || strerror(errno)) + "\n"
 *
 * Behavior:
 * - If `obj` is non-NULL, it is printed followed by ": ".
 * - If `error` is non-NULL, it is printed; otherwise `strerror(errno)` is used.
 * - Always appends a trailing newline.
 * - Does not modify `errno`.
 *
 * Parameters:
 * - obj: Optional object/context name (e.g., command/path) to prefix the error.
 * - error: Optional explicit error message; if NULL, uses `strerror(errno)`.
 */
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

/**
 * output_error_and_exit
 * Prints a formatted error message (see output_error) and terminates the process.
 *
 * Behavior:
 * - Calls `output_error(obj, error)` to print the error to stderr.
 * - If `shell` is non-NULL, calls `shell->free(shell)` to release resources.
 * - Exits the process with `exit_error_code` via `exit()`.
 *
 * Parameters:
 * - obj: Optional object/context name (e.g., command/path) to prefix the error.
 * - error: Optional explicit error message; if NULL, uses `strerror(errno)`.
 * - shell: Optional shell instance; if provided, it will be freed before exit.
 * - exit_error_code: Process exit status code.
 */
void	output_error_and_exit(const char *obj, char *error, t_shell *shell,
		int exit_error_code)
{
	output_error(obj, error);
	if (shell)
		shell->free(shell);
	exit(exit_error_code);
}
