/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_core.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 17:47:26 by ybutkov           #+#    #+#             */
/*   Updated: 2025/11/09 17:56:26 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "get_next_line.h"
#include "libft.h"
#include "shell.h"
#include "shell_internal.h"
#include "utils.h"
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>

static void	free_shell_node_bridge(void *content)
{
	t_shell_node	*node;

	if (!content)
		return ;
	node = (t_shell_node *)content;
	if (node && node->free)
		node->free(node);
}

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

static void	execute_shell(t_shell *shell)
{
	t_ast_node	*root;

	root = shell->ast->get_root(shell->ast);
	execute_shell_node(root, shell, STDIN_FILENO, STDOUT_FILENO);
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
	shell->build = build_shell;
	shell->execute = execute_shell;
	return (shell);
}
