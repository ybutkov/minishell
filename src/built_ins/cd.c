/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 16:26:49 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/02 17:34:49 by ashadrin         ###   ########.fr       */
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

int	bi_cd(t_env *envp, char **args)
{
	char	*target;
	char	*prev_dir;
	char	*cur_dir;
	
	if (args[2])
	{
		printf("minishell: cd: too many arguments\n");
		return (1);
	}
	target = set_target(envp, args);
	prev_dir = ft_strdup(getcwd(NULL, BUFFER_PATH));
	if (chdir(target) != 0)
	{
		printf("minishell: cd: %s: No such file or drectory\n", target);
		return (1);
	}
	cur_dir = getcwd(NULL, BUFFER_PATH);
	set_env_par(envp, "OLDPWD", prev_dir);
	set_env_par(envp, "PWD", cur_dir);
	free(prev_dir);
	free(cur_dir);
	return (0);
}

void	set_env_par(t_env *envp, char *key, char *value)
{
	t_env	*cur;
	t_env	*new;

	cur = envp;
	while (cur->next != NULL)
	{
		if (ft_strcmp(cur->key, key) == 0)
		{
			free(cur->value);
			cur->value = ft_strdup(value);
			return ;
		}
		cur = cur->next;
	}
	//if doesn't exist yet
	new = malloc(sizeof(t_env));
	if (!new)
		return ;
	new->key = strdup(key);
	new->value = strdup(value);
	new->next = NULL;
	cur->next = new;
}

char	*set_target(t_env *envp, char **args)
{
	char	*target;
	char	*home;
	
	if (!args[1] //only cd
			|| ft_strcmp(args[1], "~") == 0)
		target = env_value(envp, "HOME");
	else if (ft_strcmp(args[1], "-") == 0)
		target = env_value(envp, "OLDPWD");
	else if (args[1][0] == '~'
		&& args[1][1] == '/')
	{
		home = env_value(envp, "HOME");
		target = ft_strjoin(home, args[1] + 1);
	}
	else
		target = ft_strdup(args[1]);
	return (target);
}

char	*env_value(t_env *envp, char *target)
{
	t_env *cur;
	
	cur = envp;
	while (cur != NULL)
	{
		if (strcmp(cur->key, target) == 0)
			return (cur->value);
		cur = cur->next;
	}
	return (NULL);
}
