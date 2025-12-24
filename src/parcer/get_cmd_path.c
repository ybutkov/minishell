/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 13:03:41 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/24 03:37:09 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "envp_copy.h"
#include "constants.h"
#include "utils.h"

// static char	*default_getenv(void)
// {
// 	return ("/usr/local/sbin:/usr/local/bin:/usr/sbin:
// 		/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin:");
// }

static char	*build_candidate(char *path, char *cmd_name)
{
	char	*with_slash;
	char	*full_candidate;

	with_slash = ft_strjoin(path, "/");
	if (!with_slash)
		return (NULL);
	full_candidate = ft_strjoin(with_slash, cmd_name);
	if (!full_candidate)
		return (free(with_slash), NULL);
	free(with_slash);
	return (full_candidate);
}

static char	*searching_through_paths(char **paths, char *cmd_name)
{
	int		i;
	char	*candidate;

	i = 0;
	while (paths[i] != NULL)
	{
		candidate = build_candidate(paths[i], cmd_name);
		if (candidate && access(candidate, X_OK) == 0)
			return (candidate);
		free(candidate);
		i++;
	}
	return (NULL);
}

char	*get_cmd_path(t_env *env, char *cmd_name)
{
	char	**paths;
	char	*candidate;
	char	*envp_paths;
	char	*valid_path;

	if (ft_strchr(cmd_name, '/') != NULL)
		return (NULL);
	candidate = build_candidate("", cmd_name);
	if (candidate && access(candidate, X_OK) == 0)
		return (candidate);
	free(candidate);
	envp_paths = env->get_value(env, PATH);
	if (envp_paths == NULL)
		envp_paths = "";
	paths = ft_split(envp_paths, ':');
	if (!paths)
		return (HANDLE_ERROR_NULL);
	valid_path = searching_through_paths(paths, cmd_name);
	free_str_array(paths);
	return (valid_path);
}
