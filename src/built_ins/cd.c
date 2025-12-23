/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 16:26:49 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/24 00:06:13 by ashadrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//with only a relative or absolute path
//absolute path - a complete path that starts with /
//relative path - a path relative to the current working directory
//(doesn't start with /)
// ~ - shell shorthand for the user's home directory
//(expand ~ to $HOME. if home id unset - error)
//just - - change to old directory
//must use chdir(const char *path) and updaye PWD and OLDPWD in env

#include "builtin_internal.h"
#include "error.h"

static char	*set_target(t_env *env, char **args)
{
	char	*target;
	char	*home;

	if (!args[1] || ft_strcmp(args[1], "~") == 0)
		target = ft_strdup(env->get_value(env, "HOME"));
	else if (ft_strcmp(args[1], "-") == 0)
		target = ft_strdup(env->get_value(env, "OLDPWD"));
	else if (args[1][0] == '~'
		&& args[1][1] == '/')
	{
		home = env->get_value(env, "HOME");
		target = ft_strjoin(home, args[1] + 1);
	}
	else
		target = ft_strdup(args[1]);
	return (target);
}

void	update_env_print_and_free(t_env *env, char *prev_dir, char **args)
{
	char	*cur_dir;

	cur_dir = getcwd(NULL, BUFFER_PATH);
	env->set_pair(env, "OLDPWD", prev_dir);
	env->set_pair(env, "PWD", cur_dir);
	if (args[1] && ft_strcmp(args[1], "-") == 0)
	{
		write(STDOUT_FILENO, cur_dir, ft_strlen(cur_dir));
		write(STDOUT_FILENO, "\n", 1);
	}
	free(prev_dir);
	free(cur_dir);
}

int	no_target_found(char *target, char *prev_dir)
{
	char	*err_msg;

	if (chdir(target) != 0)
	{
		err_msg = ft_strjoin(CD_MSG_2_COLON, target);
		output_error(err_msg, NO_SUCH_FILE_OR_DIR);
		free(err_msg);
		free(prev_dir);
		free(target);
		return (1);
	}
	return (0);
}

int	bi_cd(t_env *env, char **args)
{
	char	*target;
	char	*prev_dir;
	int		argc;

	argc = 0;
	if (args)
		while (args[argc])
			argc++;
	if (argc > 2)
	{
		output_error(MSG_CD, MSG_TOO_MANY_ARG);
		return (EXIT_FAILURE);
	}
	target = set_target(env, args);
	prev_dir = (getcwd(NULL, BUFFER_PATH));
	if (no_target_found(target, prev_dir))
		return (EXIT_FAILURE);
	update_env_print_and_free(env, prev_dir, args);
	free(target);
	return (EXIT_SUCCESS);
}
