/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 18:18:35 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/23 19:01:51 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_UTILS_H
# define SHELL_UTILS_H

void	free_str_array(char **arr);
void	clear_tmp_folder(int file_n);
char	*get_tmp_file_name(int file_n);
int		get_file_n(int delta);
void	open_file_and_dup2(char *filename, int flags, int dup_fd,
			t_shell *shell);
void	close_fds(int *fd_0, int *fd_1);
void	dup2_and_close(int oldfd, int newfd);
void	dup2_and_close_both(int in_fd, int out_fd);
void	apply_redirects(t_list *redir, t_shell *shell);
void	apply_redirect(t_cmd *cmd, t_shell *shell);

#endif
