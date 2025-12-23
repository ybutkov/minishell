/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_list_init.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 00:31:15 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/24 00:36:05 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "env_internal.h"
#include "constants.h"

static t_env_pair	*get_env_pair(t_env *env, char *key)
{
	t_env_pair	*cur;

	if (key == NULL)
		return (NULL);
	cur = env->head;
	while (cur != NULL)
	{
		if (ft_strcmp(cur->key, key) == 0)
			return (cur);
		cur = cur->next;
	}
	return (NULL);
}

static t_env_pair	*add_env_pair(t_env *env, char *key, char *value)
{
	t_env_pair	*pair;
	t_env_pair	*last;

	last = env->head;
	while (last && last->next)
		last = last->next;
	pair = malloc(sizeof(t_env_pair));
	if (!pair)
		return (HANDLE_ERROR_NULL);
	pair->key = strdup(key);
	pair->value = NULL;
	if (value)
		pair->value = strdup(value);
	pair->next = NULL;
	if (last)
		last->next = pair;
	else
		env->head = pair;
	return (pair);
}

void	set_env_pair(t_env *env, char *key, char *value)
{
	t_env_pair	*cur;

	cur = get_env_pair(env, key);
	if (cur)
	{
		if (value)
		{
			free(cur->value);
			cur->value = ft_strdup(value);
		}
		return ;
	}
	add_env_pair(env, key, value);
}

char	*get_value(t_env *env, char *key)
{
	t_env_pair	*cur;

	cur = get_env_pair(env, key);
	// return copy !!!
	if (cur)
		return (cur->value);
	return (NULL);
}

t_env	*create_env(void)
{
	t_env	*env;

	env = malloc(sizeof(t_env));
	if (!env)
		return (NULL);
	env->head = NULL;
	env->fill_from_array = copying;
	env->to_array = copying_back;
	env->free = free_env_list;
	env->set_pair = set_env_pair;
	env->get_value = get_value;
	env->remove_pair = remove_pair;
	env->amount_vars = length_of_list;
	return (env);
}
