/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_internal.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 04:11:43 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/18 04:27:12 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_INTERNAL_H

# define ENV_INTERNAL_H
# include "envp_copy.h"

void			free_env_list(t_env *env);
int				remove_pair(t_env *env, char *key);
t_env_pair		*copying(struct s_env *env, char **envp);
int				length_of_list(t_env *env);
char			**copying_back(t_env *env);
char			*create_line(t_env_pair *node);
void			set_env_pair(t_env *env, char *key, char *value);
char			*get_value(t_env *envp, char *target);

#endif
