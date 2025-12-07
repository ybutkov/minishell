/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_internal.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 16:47:16 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/07 03:19:39 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_INTERNAL_H
# define BUILTIN_INTERNAL_H

# include <unistd.h>
# include <stdio.h>
# include "libft.h"
# include "constants.h"
# include "shell.h"
# include "envp_copy.h"

typedef struct s_export_par
{
	char	*key;
	char	*value;
}	t_export_par;

typedef int	(*t_builtin_wrapper)(t_cmd *cmd, t_shell *shell, int in_fd,
	int out_fd);

//wrappers
t_builtin_wrapper	bi_function(int bi_func);

// echo
int			bi_echo(char **args);
// cd
int			bi_cd(t_env *envp, char **args);
// pwd
int			bi_pwd(void);
// export
int			bi_export(t_env *envp, char **args);
// unset
int			bi_unset(t_env *env, char **args);
// env
int			bi_env(t_env *envp);
// exit
int			bi_exit(t_shell *shell, char **args);

#endif
