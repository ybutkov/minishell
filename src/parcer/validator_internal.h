/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator_internal.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 03:02:14 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/25 22:04:50 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VALIDATOR_INTERNAL_H
# define VALIDATOR_INTERNAL_H

# include "parsing.h"

const char	*get_token_str(t_token *token);
int			check_paren_balance(t_token *curr, int paren_depth);
int			check_empty_parens(t_token *curr);
int			check_redir_target(t_token *curr);
int			check_operators(t_token *curr, t_token *prev);
int			check_paren_place(t_token *curr, t_token *prev);
int			is_token_type(t_token *token, t_token_type *types);
int			is_token_operator(t_token *token);
int			is_redir(t_token *token);
int			syntax_error_token(t_token *token);

#endif
