/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_internal.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 16:47:16 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/23 19:07:11 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_INTERNAL_H
# define BUILTIN_INTERNAL_H

# include "constants.h"
# include "envp_copy.h"
# include "libft.h"
# include "shell.h"
# include <stdio.h>
# include <unistd.h>

typedef struct s_export_par
{
	char			*key;
	char			*value;
}					t_export_par;

typedef int			(*t_builtin_wrapper)(t_cmd *cmd, t_shell *shell, int in_fd,
				int out_fd);

t_builtin_wrapper	bi_function(int bi_func);
int					wrapper_pwd(t_cmd *cmd, t_shell *shell, int in_fd,
						int out_fd);
int					wrapper_cd(t_cmd *cmd, t_shell *shell, int in_fd,
						int out_fd);
int					wrapper_echo(t_cmd *cmd, t_shell *shell, int in_fd,
						int out_fd);
int					wrapper_env(t_cmd *cmd, t_shell *shell, int in_fd,
						int out_fd);

int					bi_echo(char **args);
int					bi_cd(t_env *envp, char **args);
int					bi_pwd(void);
int					bi_export(t_env *envp, char **args);
int					bi_unset(t_env *env, char **args);
int					bi_env(t_env *envp);
int					bi_exit(t_shell *shell, char **args);

#endif
