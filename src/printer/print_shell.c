/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_shell.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 00:00:00 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/24 17:22:32 by ashadrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "printing.h"

void	print_command(t_shell_node *shell_node, int depth)
{
	int	i;

	printf(" (");
	if (shell_node->data.cmd->argv && shell_node->data.cmd->argv[0])
	{
		i = 0;
		while (shell_node->data.cmd->argv[i])
		{
			printf("%s", shell_node->data.cmd->argv[i]);
			if (shell_node->data.cmd->argv[i + 1])
				printf(" ");
			i++;
		}
	}
	printf(")\n");
	print_cmd_details(shell_node->data.cmd, depth);
}

void	print_node_content(t_shell_node *shell_node, int depth)
{
	if (!shell_node)
	{
		printf("(null shell_node)\n");
		return ;
	}
	printf("%s", get_node_type_name(shell_node->type));
	if (shell_node->type == NODE_CMD && shell_node->data.cmd)
		print_command(shell_node, depth);
	else if ((shell_node->type == NODE_REDIR_IN
			|| shell_node->type == NODE_REDIR_OUT
			|| shell_node->type == NODE_REDIR_APPEND
			|| shell_node->type == NODE_REDIR_HEREDOC)
		&& shell_node->data.redir)
	{
		printf(" (%s %s)\n", get_redir_type_name(shell_node->data.redir->type),
			shell_node->data.redir->target);
	}
	else
		printf("\n");
}

void	print_ast_tree(t_ast_node *node, int depth, char *prefix)
{
	t_shell_node	*shell_node;

	if (!node)
		return ;
	printf("%s", prefix);
	if (node->content)
	{
		shell_node = (t_shell_node *)node->content;
		print_node_content(shell_node, depth);
	}
	else
		printf("(empty node)\n");
	if (node->left)
	{
		printf("%*s├─L: ", depth * 2, "");
		print_ast_tree(node->left, depth + 1, "");
	}
	if (node->right)
	{
		printf("%*s└─R: ", depth * 2, "");
		print_ast_tree(node->right, depth + 1, "");
	}
}

void	print_shell_tree(t_shell *shell)
{
	t_ast_node	*root;

	if (!shell)
	{
		printf("Error: shell is NULL\n");
		return ;
	}
	if (!shell->ast)
	{
		printf("Error: shell->ast is NULL\n");
		return ;
	}
	root = shell->ast->get_root(shell->ast);
	if (!root)
	{
		printf("Shell tree is empty (root is NULL)\n");
		return ;
	}
	printf("=== Shell AST Tree ===\n");
	printf("Root: ");
	print_ast_tree(root, 1, "");
	printf("======================\n");
}
