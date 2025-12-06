/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_copy.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 13:35:38 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/06 18:20:19 by ashadrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVP_COPY_H
# define ENVP_COPY_H
# include "libft.h"
# include "utils.h"

typedef struct	s_env
{
	char			*value;
	char			*key;
	struct s_env	*next;
}	t_env;

typedef struct s_env_func
{
	t_env	**envp;
	
	t_env	*(*copying)(char **envp, t_env **head);
	char	**(*copying_back)(t_env *envp);
	void	(*free_env_list)(t_env *envp);
	void	(*set_env_par)(t_env *envp, char *key, char *value);
	char	*(*env_value)(t_env *envp, char *target);
	
}	t_env_func;


t_env		*copying(char **envp, t_env **head);
void		create_node(char *line, t_env **envp);
char		**copying_back(t_env *envp);
char		*create_line(t_env *node);
int			length_of_list(t_env *envp);
void		free_env_list(t_env *envp);
t_env_func	*envp_init();
void		set_env_par(t_env *envp, char *key, char *value);
char		*env_value(t_env *envp, char *target);

#endif