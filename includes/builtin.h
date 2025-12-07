/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 16:47:16 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/07 02:04:12 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "constants.h"
# include "envp_copy.h"
# include "shell.h"
# include "shell_node.h"
# include "libft.h"
# include "shell.h"
# include <stdio.h>
# include <unistd.h>

typedef struct s_export_par
{
	char	*key;
	char	*value;
}			t_export_par;

int			builtin(int bi_func, t_cmd *cmd, t_shell *shell, int in_fd,
				int out_fd);

// echo
int			bi_echo(char **args);
int			only_ns(char *string);
// cd
int			bi_cd(t_env *envp, char **args);
void		set_env_par(t_env *envp, char *key, char *value);
char		*set_target(t_env *envp, char **args);
char		*env_value(t_env *envp, char *target);
// pwd
int			bi_pwd(void);
// export
int			bi_export(t_env *envp, char **args);
void		split_key(char *line, t_export_par *var);
int			is_valid_key(char *key);
void		print_export_sorted(void);
// unset
int			bi_unset(t_env **envp, char **args);
int			remove_var(t_env **envp, char *key);
// env
int			bi_env(t_env *envp);
// exit
int			bi_exit(t_shell *shell, char **args);
int			is_good_numeric(char *string);

#endif
