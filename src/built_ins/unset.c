/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 23:18:22 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/02 13:11:22 by ashadrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//args - any number of arguments that need to be unset
#include "builtin_internal.h"

int	bi_unset(t_env **envp, char **args)
{
	int	i;
	int	any_invalid;
	
	if (!envp || !args)
		return (1);
	i = 1;
	any_invalid = 0;
	while (args[i])
	{
		if (!remove_var(&envp, args[i]))
			any_invalid = 1;
		i++;
	}
	return (any_invalid);
}

int	remove_var(t_env **envp, char *key)
{
	t_env	*cur;
	t_env	*prev;
	
	if (!*envp ||!key)
		return (0);
	cur = *envp;
	prev = NULL;
	while (cur->next != NULL)
	{
		if (ft_strcmp(cur->key, key) == 0)
		{
			if (!prev)
				*envp = cur->next;
			else
			prev->next = cur->next;
			free(cur->key);
			if (cur->value)
				free(cur->value);
			free(cur);
			return (1); //success
		}
		prev = cur;
		cur = cur->next;
	}
	return (0);
}