/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_copy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 16:26:06 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/04 20:46:36 by ashadrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "envp_copy.h"

t_env	*copying(char **envp)
{
	int		i;
	t_env	**head;
	
	if (!envp)
		return (NULL);
		head = NULL;
	i = 0;
	while(envp[i])
	{
		create_node(envp[i], &envp);
		i++;
	}
	return (head);
}

void	create_node(char *line, t_env **envp)
{
	char	*split_sign;
	t_env	*var;
	t_env	*node;

	var = malloc(sizeof(t_env));
	if (!var)
		return ;
	var->key = NULL;
	var->value = NULL;
	var->next = NULL;
	split_sign = ft_strchr(line, '=');
	var->key = ft_substr(line, 0, split_sign - line);
	var->value = strdup(split_sign + 1);
	if (!*envp)
		*envp = var;
	{
		node = *envp;
		while (node->next != NULL)
			node = node->next;
		node->next = node;
	}
}

char	*copying_back(t_env **envp)
{
	
}