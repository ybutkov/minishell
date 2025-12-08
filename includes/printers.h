/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printers.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 00:00:00 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/07 19:28:58 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRINTERS_H
# define PRINTERS_H

# include "shell.h"
# include "ast.h"
# include "parsing.h"

void	print_shell_tree(t_shell *shell);
void	print_ast_tree(t_ast_node *node, int depth, char *prefix);
void	print_tokens_brief_once(t_token *toks, t_env *env);

#endif
