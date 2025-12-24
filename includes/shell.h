/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 17:24:57 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/24 03:52:49 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

# include "ast.h"
# include "parsing.h"
# include "shell_node.h"
# include "constants.h"

typedef struct s_shell
{
	t_ast		*ast;
	t_ctx		*ctx;

	void		(*build)(struct s_shell *shell, t_token **token);
	void		(*free)(struct s_shell *shell);
	void		(*clear)(struct s_shell *shell);
	int			(*execute)(struct s_shell *shell);
	int			(*collect_heredoc)(struct s_shell *shell);
}				t_shell;

t_shell			*create_shell(char **envp);
t_cmd			*create_cmd(char **argv, char *path);
t_shell_node	*create_shell_node(t_node_type type, void *data);
t_redir			*create_redir(t_redir_type type, char *target,
					t_quotes_status quotes);
t_ctx			*create_ctx(char **envp);

#endif
