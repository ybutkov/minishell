/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 22:15:16 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/06 22:46:29 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "envp_copy.h"
#include "parsing.h"

char	*expand_piece(t_piece *piece, t_env *envp, int last_exit_status)
{
	char	*result;
	int		i;
	char	*var_value;

	result = ft_strdup("");
	i = 0;
	while (piece->text[i])
	{
		if (piece->text[i] == '$')
		{
			var_value = expand_variable(piece->text, &i, envp,
					last_exit_status);
			// append var_value to result
		}
		else
		{
			// append regular character to result
			i++;
		}
	}
	return (result);
}

char **expand_and_split_token(t_token *token, t_env *envp, int last_exit_status)
{
	t_piece	*piece;
	char	**result_args;
	int		arg_count;
	char	*expanded;
	char	**words;

	result_args = NULL;
	arg_count = 0;
	piece = token->pieces;
	while (piece)
	{
		if (piece->has_env_v && piece->quotes != SINGLE_Q)
		{
			expanded = expand_piece(piece, envp, last_exit_status);
			if (piece->quotes == NO_QUOTES)
			{
				words = ft_split(expanded, ' ');
			    result_args = append_words(result_args, words, &arg_count);
			}
			else
			{
				result_args = append_single(result_args, expanded, &arg_count);
			}
			free(expanded);
		}
		else
		{
		    result_args = append_single(result_args, piece->text, &arg_count);
		}
		piece = piece->next;
	}

    return (result_args);
}
