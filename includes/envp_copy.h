/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_copy.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 13:35:38 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/18 04:10:50 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVP_COPY_H
# define ENVP_COPY_H
# include "libft.h"
# include "utils.h"

typedef struct s_env_pair
{
	char				*value;
	char				*key;
	struct s_env_pair	*next;
}						t_env_pair;

typedef struct s_env
{
	t_env_pair		*head;

	t_env_pair		*(*fill_from_array)(struct s_env *env, char **envp);
	char			**(*to_array)(struct s_env *env);
	void			(*set_pair)(struct s_env *env, char *key, char *value);
	char			*(*get_value)(struct s_env *env, char *key);
	int				(*remove_pair)(struct s_env *env, char *key);
	int				(*amount_vars)(struct s_env *env);
	void			(*free)(struct s_env *env);
}					t_env;

// t_env_pair			*copying(struct s_env *env, char **envp);
void				create_node(char *line, t_env_pair **envp);
// void				free_env_list(t_env *env);
// int					length_of_list(t_env *env);
// char				**copying_back(t_env *env);
char				*create_line(t_env_pair *node);
t_env				*create_env(void);
// void				set_env_pair(t_env *env, char *key, char *value);
// char				*get_value(t_env *envp, char *target);

#endif
