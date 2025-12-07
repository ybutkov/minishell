/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 16:26:49 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/07 03:18:52 by ybutkov          ###   ########.fr       */
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

static char	*set_target(t_env *env, char **args)
{
	char	*target;
	char	*home;

	if (!args[1] //only cd
			|| ft_strcmp(args[1], "~") == 0)
		target = env->get_value(env, "HOME");
	else if (ft_strcmp(args[1], "-") == 0)
		target = env->get_value(env, "OLDPWD");
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

int	bi_cd(t_env *env, char **args)
{
	char	*target;
	char	*prev_dir;
	char	*cur_dir;

	if (args[2])
	{
		printf("minishell: cd: too many arguments\n");
		return (1);
	}
	target = set_target(env, args);
	prev_dir = ft_strdup(getcwd(NULL, BUFFER_PATH));
	if (chdir(target) != 0)
	{
		printf("minishell: cd: %s: No such file or drectory\n", target);
		return (1);
	}
	cur_dir = getcwd(NULL, BUFFER_PATH);
	env->set_pair(env, "OLDPWD", prev_dir);
	env->set_pair(env, "PWD", cur_dir);
	free(prev_dir);
	free(cur_dir);
	return (0);
}
