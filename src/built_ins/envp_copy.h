/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_copy.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 13:35:38 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/04 18:57:37 by ashadrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVP_COPY_H
# define ENVP_COPY_H

#include "builtin_internal.h"

typedef struct	s_env
{
	char			*value;
	char			*key;
	struct s_env	*next;
}	t_env;

t_env	*copying(char **envp);
void	create_node(char *line, t_env **envp);
//should also have freeing the list and transforming it back
#endif