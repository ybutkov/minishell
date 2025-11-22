/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_fixtures.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 15:24:25 by ybutkov           #+#    #+#             */
/*   Updated: 2025/11/22 18:03:20 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_FIXTURES_H
# define TOKEN_FIXTURES_H

# include "parsing.h"

t_token *create_token(t_token_type type, char *value);
t_token	*tokens_echo_grep_redirect(void);
t_token	*tokens_heredoc_sed_append(void);
t_token	*tokens_export_and_echo_or(void);
t_token	*tokens_group_and_pipe(void);
t_token	*tokens_quotes_semi_append(void);
t_token	*tokens_simple_command(void);
t_token	*tokens_input_redirect(void);
t_token	*tokens_simple_pipe(void);
t_token	*tokens_and_commands(void);
t_token	*tokens_or_commands(void);
t_token	*tokens_multiple_semicolon(void);
t_token	*tokens_pipe_with_redirects(void);
t_token	*tokens_redirect_and_command(void);
t_token	*tokens_triple_pipe(void);
t_token *tokens_triple_pipe_error(void);
t_token	*tokens_five_pipe_chain(void);
t_token	*tokens_input_pipe_chain(void);
t_token *tokens_input_pipe_chain_1_redir(void);
t_token	*tokens_pipe_output_redirect(void);
t_token	*tokens_pipe_append_redirect(void);
t_token	*tokens_input_pipe_output(void);
t_token	*tokens_multi_pipe_multi_redirect(void);
t_token	*tokens_dual_redirect_pipe(void);
t_token	*tokens_complex_multi_redirect(void);
t_token *tokens_complex_multi_redirect_2(void);

#endif
