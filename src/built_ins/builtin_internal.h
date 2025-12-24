/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_internal.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 16:47:16 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/24 16:36:27 by ashadrin         ###   ########.fr       */
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

void				split_key(char *line, t_export_par *var, char *separator);
int					is_valid_key(char *key);
void				sort_pointers_array(t_env_pair **array, int size);
void				print_pointers_array(t_env_pair **array, int size);
void				print_export_sorted(t_env *env);
void				output_declare(t_env *env);
int					check_flags(t_env *env, char **args, int *i);
void				safe_error_output(int *status, char *arg);
void				handle_plus_equal(t_env *env, t_export_par *var,
						char *arg, int *is_valid);
void				key_value_free(t_export_par *var);

#endif
