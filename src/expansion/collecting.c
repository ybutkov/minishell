/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collecting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 21:14:18 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/23 21:31:36 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int	collect_exp_args_to_list(char **expanded_args, char **new_arg,
	t_list **arg_list)
{
	int		i;

	i = -1;
	while (expanded_args[++i])
	{
		if (ft_strcmp(expanded_args[i], " ") == 0)
		{
			ft_lstadd_back(arg_list, ft_lstnew(*new_arg));
			*new_arg = NULL;
		}
		else if (ft_strappend(new_arg, expanded_args[i]) == 0)
		{
			return (NO);
		}
	}
	return (OK);
}

int	collect_pieces_to_strings(t_shell *shell, t_token *curr_tkn,
	t_list **arg_list)
{
	char	**expanded_args;
	char	*new_arg;

	new_arg = NULL;
	if (curr_tkn->pieces)
	{
		expanded_args = expand_and_split_token(curr_tkn, shell->ctx->env,
				shell->ctx->last_exit_status);
		if (!expanded_args)
			return (free(expanded_args), NO);
		if (collect_exp_args_to_list(expanded_args, &new_arg, arg_list) == NO)
			return (NO);
		if (new_arg)
			ft_lstadd_back(arg_list, ft_lstnew(new_arg));
		free_str_array(expanded_args);
	}
	else
		ft_lstadd_back(arg_list, ft_lstnew(ft_strdup(curr_tkn->value)));
	return (OK);
}

char	*collect_pieces_to_string(t_shell *shell, t_token *curr_tkn)
{
	char	**expanded_args;
	char	*new_arg;
	int		i;

	i = 0;
	if (curr_tkn->pieces)
	{
		expanded_args = expand_and_split_token(curr_tkn, shell->ctx->env,
				shell->ctx->last_exit_status);
		new_arg = ft_strdup("");
		if (!expanded_args || new_arg == NULL)
			return (free(expanded_args), free(new_arg), NULL);
		while (expanded_args[i])
		{
			if (ft_strappend(&new_arg, expanded_args[i++]) == 0)
			{
				free_str_array(expanded_args);
				return (free(new_arg), NULL);
			}
		}
		free_str_array(expanded_args);
	}
	else
		new_arg = ft_strdup(curr_tkn->value);
	return (new_arg);
}
