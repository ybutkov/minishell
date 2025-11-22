/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_shell.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 00:00:00 by ybutkov           #+#    #+#             */
/*   Updated: 2025/11/11 17:10:16 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "print_shell.h"
#include "shell_node.h"
#include "ast.h"
#include "libft.h"
#include <stdio.h>

static const char	*get_node_type_name(t_node_type type)
{
	static const char	*names[] = {
		"CMD",
		"SEMICOLON",
		"PIPE",
		"AND",
		"OR",
		"REDIR_IN",
		"REDIR_OUT",
		"REDIR_APPEND",
		"REDIR_HEREDOC",
		"SUBSHELL"
	};

	if (type >= 0 && type <= NODE_SUBSHELL)
		return (names[type]);
	return ("UNKNOWN");
}

static const char	*get_redir_type_name(t_redir_type type)
{
	static const char	*names[] = {
		"<",
		">",
		">>",
		"<<"
	};

	if (type >= 0 && type <= REDIR_HEREDOC)
		return (names[type]);
	return ("?");
}

static void	print_cmd_details(t_cmd *cmd, int depth)
{
	int		i;
	t_list	*redir_node;
	t_redir	*redir;

	if (!cmd)
		return ;
	printf("%*s  argv: [", depth * 2, "");
	if (cmd->argv)
	{
		i = 0;
		while (cmd->argv[i])
		{
			printf("\"%s\"", cmd->argv[i]);
			if (cmd->argv[i + 1])
				printf(", ");
			i++;
		}
	}
	printf("]\n");
	if (cmd->path)
		printf("%*s  path: %s\n", depth * 2, "", cmd->path);
	if (cmd->redirs)
	{
		printf("%*s  redirections:\n", depth * 2, "");
		redir_node = cmd->redirs;
		while (redir_node)
		{
			redir = (t_redir *)redir_node->content;
			printf("%*s    %s %s\n", depth * 2, "",
				get_redir_type_name(redir->type), redir->target);
			redir_node = redir_node->next;
		}
	}
}

static void	print_node_content(t_shell_node *shell_node, int depth)
{
	int	i;

	if (!shell_node)
	{
		printf("(null shell_node)\n");
		return ;
	}
	printf("%s", get_node_type_name(shell_node->type));
	if (shell_node->type == NODE_CMD && shell_node->data.cmd)
	{
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
