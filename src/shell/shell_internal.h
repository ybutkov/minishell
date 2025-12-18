/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_internal.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 17:28:17 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/18 15:52:40 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_INTERNAL_H
# define SHELL_INTERNAL_H

# include "shell.h"

typedef struct s_token_lvl
{
	t_token_type	start;
	t_token_type	end;
	int				lvl;
}	t_token_lvl;

int			execute_pipe(t_ast_node *node, t_shell *shell, int in_fd,
				int out_fd);
int			execute_shell_node(t_ast_node *node, t_shell *shell, int in_fd,
				int out_fd);
int			execute_redir_heredoc(t_shell *shell, t_redir *redirect);
void		build_shell(t_shell *shell, t_token **token);
int			execute_cmd(t_cmd *cmd, t_shell *shell, int input_fd,
				int output_fd);
int			collect_heredoc_node(t_ast_node *node, t_shell *shell);
void		close_fds(int *fd_0, int *fd_1);
void		dup2_and_close(int oldfd, int newfd);
t_node_type	get_node_type(t_token_type token_type);

#endif
