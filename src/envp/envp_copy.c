/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_copy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 16:26:06 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/07 03:30:18 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "envp_copy.h"

t_env_pair	*copying(t_env *env, char **envp)
{
	t_env_pair	*head;
	int			i;

	if (!envp)
		return (NULL);
	head = NULL;
	i = 0;
	while (envp[i])
	{
		create_node(envp[i], &head);
		i++;
	}
	env->head = head;
	return (head);
}

void	create_node(char *line, t_env_pair **envp)
{
	char		*split_sign;
	t_env_pair	*var;
	t_env_pair	*node;

	var = malloc(sizeof(t_env_pair));
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

void	free_env_list(t_env *env)
{
	t_env_pair	*node;
	t_env_pair	*temp;

	node = env->head;
	if (!node)
		return ;
	while (node)
	{
		temp = node->next;
		free(node->key);
		free(node->value);
		free(node);
		node = temp;
	}
}
