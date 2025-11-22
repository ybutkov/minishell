/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_shell.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 00:00:00 by ybutkov           #+#    #+#             */
/*   Updated: 2025/11/11 14:24:19 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRINT_SHELL_H
# define PRINT_SHELL_H

# include "shell.h"
# include "ast.h"

void	print_shell_tree(t_shell *shell);
void	print_ast_tree(t_ast_node *node, int depth, char *prefix);

#endif
