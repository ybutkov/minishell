/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 16:35:25 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/02 12:49:13 by ashadrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_internal.h"

int	bi_env(t_env *envp)
{
	t_env *current;
	
	if (!envp)
		return (1);
	current = envp;
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
