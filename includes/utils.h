/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 20:05:36 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/23 22:50:45 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "../libft/includes/libft.h"
# include <stdlib.h>

typedef struct s_shell t_shell;

void	free_str_array(char **arr);
void	**list_to_array_generic(t_list *lst, void *(*convert)(void *));
char	**list_to_array(t_list *lst);
int		is_char_space(char ch);
void	empty_func(void *p);

int		open_file(char *filename, int flags, t_shell *shell);
void	open_file_and_dup2(char *filename, int flags, int dup_fd,
			t_shell *shell);
void	dup2_and_close_both(int in_fd, int out_fd);
void	dup2_and_close(int oldfd, int newfd);
void	close_fds(int *fd_0, int *fd_1);

#endif
