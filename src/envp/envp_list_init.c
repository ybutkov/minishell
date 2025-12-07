/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_list_init.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 00:31:15 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/07 03:07:14 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "envp_copy.h"

int	remove_pair(t_env *env, char *key)
{
	t_env_pair	*cur;
	t_env_pair	*prev;

	if (!env->head || !key)
		return (0);
	cur = env->head;
	prev = NULL;
	while (cur->next != NULL)
	{
		if (ft_strcmp(cur->key, key) == 0)
		{
			if (!prev)
				env->head = cur->next;
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

void	set_env_pair(t_env *env, char *key, char *value)
{
	t_env_pair	*cur;
	t_env_pair	*new;

	cur = env->head;
	while (cur->next != NULL)
	{
		if (ft_strcmp(cur->key, key) == 0)
		{
			free(cur->value);
			cur->value = ft_strdup(value);
			return ;
		}
		cur = cur->next;
	}
	//if doesn't exist yet
	new = malloc(sizeof(t_env_pair));
	if (!new)
		return ;
	new->key = strdup(key);
	new->value = strdup(value);
	new->next = NULL;
	cur->next = new;
}

char	*get_value(t_env *env, char *key)
{
	t_env_pair	*cur;

	cur = env->head;
	while (cur != NULL)
	{
		if (strcmp(cur->key, key) == 0)
			return (cur->value);
		cur = cur->next;
	}
	return (NULL);
}

t_env	*create_env(void)
{
	t_env	*env;

	env = malloc(sizeof(t_env));
	if (!env)
		return (NULL);
	env->head = NULL;
	env->copying = copying;
	env->copying_back = copying_back;
	env->free = free_env_list;
	env->set_pair = set_env_pair;
	env->get_value = get_value;
	env->remove_pair = remove_pair;
	env->amount_vars = length_of_list;
	return (env);
}
