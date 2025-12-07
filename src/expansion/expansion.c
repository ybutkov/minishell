/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 22:15:16 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/07 19:36:34 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "envp_copy.h"
#include "parsing.h"
#include "constants.h"

char	*expand_piece(t_piece *piece, t_env *env, int last_exit_status)
{
	char	*result;
	char	*key;
	char	*value;

	result = ft_strdup("");
	if (piece->text[0] == '$')
	{
		key = piece->text + 1;
		// Handle $?
		// $$ and others ???
		if (ft_strcmp(key, "?") == 0)
			value = ft_itoa(last_exit_status);
		else
			value = env->get_value(env, key);
		if (ft_strappend(&result, value) != 1)
			return (HANDLE_ERROR_NULL);
	}
	else
	{
		if (ft_strappend(&result, piece->text) != 1)
			return (HANDLE_ERROR_NULL);
	}
	return (result);
}

int	add_list_array(t_list *res_list, char **elems)
{
	t_list	*new;
	int		n;

	n = 0;
	while (elems[n])
	{
		new = ft_lstnew(elems[n]);
		if (new == NULL)
			return (HANDLE_ERROR_MINUS_ONE);
		ft_lstadd_back(&res_list, new);
		n++;
	}
	return (n);
}

char **expand_and_split_token(t_token *token, t_env *env, int last_exit_status)
{
	t_piece	*piece;
	t_list	*res_list;
	int		amount;
	char	**result;
	char	*expanded;
	char	**words;

	res_list = NULL;
	piece = token->pieces;
	while (piece)
	{
		if (piece->has_env_v && piece->quotes != SINGLE_Q)
		{
			expanded = expand_piece(piece, env, last_exit_status);
			if (piece->quotes == NO_QUOTES)
			{
				words = ft_split(expanded, ' ');
				amount = add_list_array(res_list, words);
				if (amount == -1)
					return (HANDLE_ERROR_NULL);
			}
			else
			{
				ft_lstadd_back(&res_list, ft_lstnew(expanded));
			}
			free(expanded);
		}
		else
		{
			ft_lstadd_back(&res_list, ft_lstnew(ft_strdup(piece->text)));
		}
		piece = piece->next;
	}
	result = list_to_array(res_list);
	// ft_lstclear(&res_list, free);
	return (result);
}
