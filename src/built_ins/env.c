/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 16:35:25 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/15 21:54:59 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_internal.h"
#include <unistd.h>

int	bi_env(t_env *env)
{
	t_env_pair	*current;
	int			len;

	if (!env->head)
		return (1);
	current = env->head;
	while (current != NULL)
	{
		if (current->key)
		{
			len = ft_strlen(current->key);
			write(STDOUT_FILENO, current->key, len);
			write(STDOUT_FILENO, "=", 1);
			len = ft_strlen(current->value);
			write(STDOUT_FILENO, current->value, len);
			write(STDOUT_FILENO, "\n", 1);
			// printf("%s=", current->key);
			// printf("%s\n", current->value);
		}
		current = current->next;
	}
	return (0);
}
