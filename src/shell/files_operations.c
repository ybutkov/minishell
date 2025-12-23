/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files_operations.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:08:32 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/23 19:08:45 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "error.h"
#include <fcntl.h>

void	close_fds(int *fd_0, int *fd_1)
{
	if (fd_0 && *fd_0 >= 0)
	{
		close(*fd_0);
		*fd_0 = -1;
	}
	if (fd_1 && *fd_1 >= 0)
	{
		close(*fd_1);
		*fd_1 = -1;
	}
}

void	dup2_and_close(int oldfd, int newfd)
{
	dup2(oldfd, newfd);
	close(oldfd);
}

void	dup2_and_close_both(int in_fd, int out_fd)
{
	if (in_fd != STDIN_FILENO)
		dup2_and_close(in_fd, STDIN_FILENO);
	if (out_fd != STDOUT_FILENO)
		dup2_and_close(out_fd, STDOUT_FILENO);
}

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
	dup2_and_close(fd, dup_fd);
}

