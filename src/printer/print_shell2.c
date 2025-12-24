/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_shell2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 17:13:52 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/24 17:23:52 by ashadrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "printing.h"

const char	*get_node_type_name(t_node_type type)
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

const char	*get_redir_type_name(t_redir_type type)
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

void	print_list(t_cmd *cmd)
{
	int	i;

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
}

void	print_cmd_details(t_cmd *cmd, int depth)
{
	t_list	*redir_node;
	t_redir	*redir;

	if (!cmd)
		return ;
	printf("%*s  argv: [", depth * 2, "");
	print_list(cmd);
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
