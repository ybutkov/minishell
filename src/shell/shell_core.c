/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_core.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 17:47:26 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/24 04:36:22 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "libft.h"
#include "shell_internal.h"
#include "utils.h"
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include <limits.h>

static void	free_shell(t_shell *shell)
{
	if (!shell)
		return ;
	if (shell->ast)
	{
		shell->ast->free(shell->ast, free_shell_node_bridge);
		shell->ast = NULL;
	}
	if (shell->ctx)
	{
		shell->ctx->free_ctx(shell->ctx);
		shell->ctx = NULL;
	}
	free(shell);
}

static void	clear_shell(t_shell *shell)
{
	if (!shell)
		return ;
	if (shell->ast)
	{
		shell->ast->free(shell->ast, free_shell_node_bridge);
		shell->ast = NULL;
	}
	shell->ast = create_ast_tree();
}

static int	execute_shell(t_shell *shell)
{
	t_ast_node	*root;
	int			status;

	root = shell->ast->get_root(shell->ast);
	if (!root || !root->get_content(root))
	{
		shell->ctx->last_exit_status = 2;
		return (2);
	}
	status = execute_shell_node(root, shell, STDIN_FILENO, STDOUT_FILENO);
	shell->ctx->last_exit_status = status;
	return (shell->ctx->last_exit_status);
}

static int	collect_heredocs(t_shell *shell)
{
	t_ast_node	*root;
	int			status;

	root = shell->ast->get_root(shell->ast);
	if (!root || !root->get_content(root))
		return (1);
	status = collect_heredoc_node(root, shell);
	return (status);
}

t_shell	*create_shell(char **envp)
{
	t_shell	*shell;
	t_ctx	*ctx;

	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	ctx = create_ctx(envp);
	if (!ctx)
	{
		free(shell);
		return (NULL);
	}
	shell->ctx = ctx;
	shell->ast = create_ast_tree();
	if (!shell->ast)
	{
		free(shell);
		return (NULL);
	}
	shell->free = free_shell;
	shell->clear = clear_shell;
	shell->build = build_shell;
	shell->execute = execute_shell;
	shell->collect_heredoc = collect_heredocs;
	return (shell);
}
