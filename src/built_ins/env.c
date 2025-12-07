/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 16:35:25 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/07 02:55:40 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_internal.h"

int	bi_env(t_env *env)
{
	t_env_pair	*current;

	if (!env->head)
		return (1);
	current = env->head;
	while (current != NULL)
	{
		if (current->key)
		{
			printf("%s=", current->key);
			printf("%s\n", current->value);
		}
		current = current->next;
	}
	return (0);
}
