/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_node.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 17:25:11 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/22 20:14:52 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_NODE_H
# define SHELL_NODE_H

# include "libft.h"
# include "envp_copy.h"
# include "parsing.h"

typedef enum e_node_type
{
	NODE_CMD,
	NODE_SEMICOLON,
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
	NODE_REDIR_IN,
	NODE_REDIR_OUT,
	NODE_REDIR_APPEND,
	NODE_REDIR_HEREDOC,
	NODE_SUBSHELL
}					t_node_type;

typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}					t_redir_type;

typedef struct s_ctx
{
	char			**envp;
	t_env			*env;
	int				last_exit_status;
	int				should_exit;

	void			(*free_ctx)(struct s_ctx *ctx);
}					t_ctx;

typedef struct s_redir
{
	t_redir_type	type;
	char			*target;
	e_quotes_status	quotes;

	void			(*free_redir)(struct s_redir *redir);
}					t_redir;

typedef struct s_cmd
{
	char			**argv;
	char			*path;
	t_list			*redirs;
	t_token			*tokens;

	void			(*free_cmd)(struct s_cmd *cmd);
}					t_cmd;

typedef struct s_shell_node
{
	t_node_type		type;
	union			u_data
	{
		t_cmd		*cmd;
		t_redir		*redir;
	} data;
	t_list			*redirs;
	void			(*free)(struct s_shell_node *node);
}					t_shell_node;

#endif
