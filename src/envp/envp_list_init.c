/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_list_init.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 00:31:15 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/06 00:53:52 by ashadrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "envp_copy.h"

t_env_func	*envp_init(void)
{
	t_env_func	*env;

	env = malloc(sizeof(t_env_func));
	if (!env)
		return (NULL);
	env->envp = NULL;
	env->copying = copying;
	env->copying_back = copying_back;
	env->free_env_list = free_env_list;
	env->set_env_par = set_env_par;
	env->env_value = env_value;
	return (env);
}
