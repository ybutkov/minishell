/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_to_array.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 00:30:17 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/06 19:43:29 by ashadrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "envp_copy.h"

char	**copying_back(t_env *envp)
{
	int		len;
	char	**array;
	t_env	*node;
	int		i;

	len = length_of_list(envp);
	array = malloc(sizeof(char *) * (len + 1));
	if (!array)
		return (NULL);
	node = envp;
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

char	*create_line(t_env *node)
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

int		length_of_list(t_env *envp)
{
	int		i;
	t_env	*node;
	
	i = 0;
	node = envp;
	while (node != NULL)
	{
		node = node->next;
		i++;
	}
	return (i);
}
