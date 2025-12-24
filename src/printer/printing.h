/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printing.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 17:16:39 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/24 17:23:23 by ashadrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRINTING_H
# define PRINTING_H

# include "printers.h"
# include "shell_node.h"
# include "ast.h"
# include "libft.h"
# include <stdio.h>

const char	*get_node_type_name(t_node_type type);
const char	*get_redir_type_name(t_redir_type type);
void		print_list(t_cmd *cmd);
void		print_cmd_details(t_cmd *cmd, int depth);
void		print_command(t_shell_node *shell_node, int depth);
void		print_node_content(t_shell_node *shell_node, int depth);
void		print_ast_tree(t_ast_node *node, int depth, char *prefix);
void		print_shell_tree(t_shell *shell);

#endif