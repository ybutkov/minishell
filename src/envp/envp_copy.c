/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_copy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 16:26:06 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/06 18:16:08 by ashadrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "envp_copy.h"

t_env	*copying(char **envp, t_env **head)
{
	int		i;
	
	if (!envp || !head)
		return (NULL);
	*head = NULL;
	i = 0;
	while(envp[i])
	{
		create_node(envp[i], head);
		i++;
	}
	return (*head);
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
	var->value = ft_strdup(split_sign + 1);
	if (!*envp)
		*envp = var;
	else
	{
		node = *envp;
		while (node->next != NULL)
			node = node->next;
		node->next = var;
	}
}

void	free_env_list(t_env *envp)
{
	t_env	*node;
	t_env	*temp;
	
	if (!envp)
		return ;
	node = envp;
	while (node)
	{
		temp = node->next;
		free(node->key);
		free(node->value);
		free(node);
		node = temp;
	}
	envp = NULL;
}
