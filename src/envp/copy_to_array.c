/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_to_array.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 00:30:17 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/18 04:23:57 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env_internal.h"

char	**copying_back(t_env *env)
{
	int			len;
	char		**array;
	t_env_pair	*node;
	int			i;

	len = env->amount_vars(env);
	array = malloc(sizeof(char *) * (len + 1));
	if (!array)
		return (NULL);
	node = env->head;
	i = 0;
	while (node)
	{
		array[i] = create_line(node);
		if (!array[i])
			return (free_str_array(array), NULL);
		node = node->next;
		i++;
	}
	array[i] = NULL;
	return (array);
}

char	*create_line(t_env_pair *node)
{
	int		linelen;
	char	*line;
	int		len_key;
	int		len_value;

	if (!node->key)
		return (NULL);
	if (!node->value)
		len_value = 0;
	else
		len_value = ft_strlen(node->value);
	len_key = ft_strlen(node->key);
	linelen = len_key + len_value + 2;
	line = malloc(sizeof(char) * linelen);
	if (!line)
		return (NULL);
	ft_memcpy(line, node->key, len_key);
	line[len_key] = '=';
	if (len_value)
		ft_memcpy(line + len_key + 1, node->value, len_value);
	line[linelen - 1] = '\0';
	return (line);
}

int	length_of_list(t_env *env)
{
	int			i;
	t_env_pair	*node;

	i = 0;
	node = env->head;
	while (node != NULL)
	{
		node = node->next;
		i++;
	}
	return (i);
}

int	remove_pair(t_env *env, char *key)
{
	t_env_pair	*cur;
	t_env_pair	*prev;

	if (!env->head || !key)
		return (0);
	cur = env->head;
	prev = NULL;
	while (cur != NULL)
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
			return (1);
		}
		prev = cur;
		cur = cur->next;
	}
	return (0);
}
