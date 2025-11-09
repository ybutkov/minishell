/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 17:24:57 by ybutkov           #+#    #+#             */
/*   Updated: 2025/11/09 17:25:00 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

# include "ast.h"
# include "shell_node.h"

typedef struct s_shell
{
	t_ast		*ast;
	t_ctx		*ctx;

	void		(*build)(struct s_shell *shell, char **commands, int argc);
	void		(*free)(struct s_shell *shell);
	void		(*execute)(struct s_shell *shell);
}				t_shell;

t_shell			*create_shell(char **envp);
t_cmd			*create_cmd(char **argv, char *path);
t_cmd			*create_cmd_from_raw_str(char *raw_command, t_shell *shell);
t_shell_node	*create_shell_node(t_node_type type, void *data);
t_redir			*create_redir(t_redir_type type, char *target);
t_ctx			*create_ctx(char **envp);

#endif
