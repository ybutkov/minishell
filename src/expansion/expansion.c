/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 22:15:16 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/24 03:53:48 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "constants.h"
#include "envp_copy.h"
#include "libft.h"
#include "parsing.h"

static char	*expand_var(t_piece *piece, t_quotes_status stat, t_env *env,
	int last_exit_status)
{
	char	*result;
	char	*key;
	char	*value;

	key = piece->text + 1;
	if (ft_strcmp(key, "?") == 0)
	{
		result = ft_itoa(last_exit_status);
		return (result);
	}
	value = env->get_value(env, key);
	if (value == NULL && stat == DOUBLE_Q)
		value = "$";
	else if (value == NULL)
		value = "";
	result = ft_strdup(value);
	return (result);
}

static char	*expand_tilde(t_piece *piece, t_env *env)
{
	char	*result;
	char	*value;

	value = env->get_value(env, HOME);
	if (value == NULL)
		value = "";
	result = ft_strdup(value);
	if (result == NULL)
		return (HANDLE_ERROR_NULL);
	if ((piece->text + 1) && ft_strappend(&result, piece->text + 1) != 1)
		return (free(result), HANDLE_ERROR_NULL);
	return (result);
}

char	*expand_piece(t_piece *piece, t_quotes_status stat, t_env *env,
	int last_exit_status)
{
	char	*result;

	if (piece->text[0] == '$')
		result = expand_var(piece, stat, env, last_exit_status);
	else if (piece->text[0] == '~')
		result = expand_tilde(piece, env);
	else
		result = ft_strdup(piece->text);
	return (result);
}

char	**expand_and_split_token(t_token *token, t_env *env,
	int last_exit_status)
{
	t_piece	*piece;
	t_list	*res_list;
	char	*expanded;
	char	**result_array;
	char	**array;
	int		i;

	res_list = NULL;
	piece = token->pieces;
	while (piece)
	{
		if ((piece->has_env_v || piece->has_tilde) && piece->quotes != SINGLE_Q)
		{
			expanded = expand_piece(piece, piece->quotes, env, last_exit_status);
			if (piece->quotes == NO_QUOTES)
			{
				array = ft_split(expanded, ' ');
				if (expanded != NULL && res_list
					&& is_char_space(expanded[0]) == OK)
					ft_lstadd_back(&res_list, ft_lstnew(ft_strdup(" ")));
				i = 0;
				while (array[i])
				{
					if (i > 0)
						ft_lstadd_back(&res_list, ft_lstnew(ft_strdup(" ")));
					ft_lstadd_back(&res_list, ft_lstnew(array[i++]));
				}
				if (expanded != NULL && i > 0
					&& is_char_space(expanded[ft_strlen(expanded) - 1]) == OK)
					ft_lstadd_back(&res_list, ft_lstnew(ft_strdup(" ")));

				free(array);
			}
			else
				ft_lstadd_back(&res_list, ft_lstnew(ft_strdup(expanded)));
			free(expanded);
		}
		else
			ft_lstadd_back(&res_list, ft_lstnew(ft_strdup(piece->text)));
		piece = piece->next;
	}
	result_array = list_to_array(res_list);
	ft_lstclear(&res_list, empty_func);
	return (result_array);
}
