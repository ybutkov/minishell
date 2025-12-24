/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_internal.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 17:28:17 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/24 02:44:19 by ybutkov          ###   ########.fr       */
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
}					t_token_lvl;

/* Shell build*/
t_ast_node			*build_ast(t_shell *shell, t_ast_node **node,
						t_token **start_tkn, t_token **end_tkn);
int					is_subshell_start(t_token *start_tkn);
int					create_node_for_subshell(t_shell *shell, t_ast_node **node,
						t_token *start_tkn, t_token *end_tkn);
int					collect_redirs(t_shell *shell, t_cmd *cmd,
						t_token **start_tkn, t_token **end_tkn);
void				add_redirect(t_cmd *cmd, t_redir *redirect);
t_token				*get_next_token_for_lvl(t_token *curr, t_token *end_token,
						t_token_lvl *token_lvl);
t_token_lvl			*get_token_lvl(int lvl);
int					check_paren(t_token **curr_token, int *opened);
void				create_leaves(t_shell *shell, t_ast_node **node,
						t_token *curr_tkn, t_token **start_end_tokens);
t_redir_type		get_only_redir_types(t_token *token);
t_redir				*create_redirect(t_shell *shell, t_redir_type type,
						t_token *curr_tkn);
void				free_shell_node_bridge(void *content);
int					collect_heredoc_input(t_redir *redir, int write_fd,
						t_shell *shell);
int					expand_heredoc_line(char **line, t_shell *shell);
void				apply_subshell_redirs(t_shell *shell,
						t_shell_node *shell_node);

int					execute_pipe(t_ast_node *node, t_shell *shell, int in_fd,
						int out_fd);
int					execute_shell_node(t_ast_node *node, t_shell *shell,
						int in_fd, int out_fd);

void				build_shell(t_shell *shell, t_token **token);
int					execute_cmd(t_cmd *cmd, t_shell *shell, int input_fd,
						int output_fd);
int					collect_heredoc_node(t_ast_node *node, t_shell *shell);
char				*collect_pieces_to_string(t_shell *shell,
						t_token *curr_tkn);
int					collect_pieces_to_strings(t_shell *shell, t_token *curr_tkn,
						t_list **arg_list);
t_node_type			get_node_type(t_token_type token_type);
int					return_status(int status);

#endif
