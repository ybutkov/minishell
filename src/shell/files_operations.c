/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files_operations.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:08:32 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/18 16:13:22 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "error.h"
#include <fcntl.h>

void	open_file_and_dup2(char *filename, int flags, int dup_fd,
		t_shell *shell)
{
	int	fd;
	int	mode;

	if (flags & O_CREAT)
		mode = 0644;
	else
		mode = 0;
	fd = open(filename, flags, mode);
	if (fd == -1)
		output_error_and_exit(filename, NULL, shell, EXIT_FAILURE);
	dup2(fd, dup_fd);
	close(fd);
}
