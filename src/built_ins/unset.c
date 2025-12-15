/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 23:18:22 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/15 18:22:11 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//args - any number of arguments that need to be unset
#include "builtin_internal.h"

int	bi_unset(t_env *env, char **args)
{
	int	i;
	int	any_invalid;

	if (!env->head || !args)
		return (1);
	i = 1;
	any_invalid = 0;
	while (args[i])
	{
		env->remove_pair(env, args[i]);
		// if (!env->remove_pair(env, args[i]))
		// 	any_invalid = 1;
		i++;
	}
	return (any_invalid);
}


