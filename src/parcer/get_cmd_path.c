/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 13:03:41 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/23 03:15:45 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "envp_copy.h"
#include "constants.h"
#include "utils.h"

static char	*default_getenv(void)
{
	return ("/usr/local/sbin:/usr/local/bin:/usr/sbin:\
		/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin:");
}

// char	*envp_loop(t_env *envp)
// {
// 	t_env_pair *cur;

// 	cur = envp->head;
// 	while (cur)
// 	{
// 		if (ft_strcmp(cur->key, "PATH") == 0)
// 			return (cur->value);
// 		cur = cur->next;
// 	}
// 	return (NULL);
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
	// if (cmd_name[0] == '/')
		// return (NULL);
	envp_paths = env->get_value(env, PATH);
	if (envp_paths == NULL)
		envp_paths = default_getenv();
	paths = ft_split(envp_paths, ':');
	if (!paths)
		return (HANDLE_ERROR_NULL);
	valid_path = searching_through_paths(paths, cmd_name);
	free_str_array(paths);
	return (valid_path);
}



// void	free_split(char **array)
// {
// 	int	i;

// 	i = 0;
// 	if (!array)
// 		return ;
// 	while (array[i])
// 	{
// 		free(array[i]);
// 		i++;
// 	}
// 	free(array);
// }



// static void	ft_free_split(char **arr);
// static char	*check_full_path(char *dir, char *cmd);

// static char	*default_getenv(void)
// {
// 	return (ft_strdup("PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:
// 		/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin:"));
// }

// static char	*get_full_path(char *cmd, char *env_paths)
// {
// 	char	*full_path;
// 	char	**paths;
// 	char	**all_paths;

// 	full_path = check_full_path("", cmd);
// 	if (full_path)
// 		return (full_path);
// 	if (cmd[0] == '/')
// 		return (NULL);
// 	all_paths = ft_split(env_paths, ':');
// 	if (!all_paths)
// 		return (NULL);
// 	paths = all_paths;
// 	while (*paths)
// 	{
// 		full_path = check_full_path(*paths, cmd);
// 		if (full_path)
// 			return (ft_free_split(all_paths), full_path);
// 		paths++;
// 	}
// 	ft_free_split(all_paths);
// 	return (NULL);
// }

// char	*get_cmd_path(char *cmd, char **envp)
// {
// 	while (*envp)
// 	{
// 		if (ft_strncmp(*envp, "PATH=", 5) == 0)
// 			return (get_full_path(cmd, (*envp + 5)));
// 		envp++;
// 	}
// 	return (get_full_path(cmd, default_getenv()));
// }

// static void	ft_free_split(char **paths)
// {
// 	char	**temp;

// 	if (!paths)
// 		return ;
// 	temp = paths;
// 	while (*temp)
// 	{
// 		free(*temp);
// 		temp++;
// 	}
// 	free(paths);
// }

// static char	*check_full_path(char *dir, char *cmd)
// {
// 	char	*full_path;
// 	int		dir_len;
// 	int		cmd_len;

// 	dir_len = ft_strlen(dir);
// 	cmd_len = ft_strlen(cmd);
// 	if (dir_len > 0)
// 		full_path = malloc(dir_len + cmd_len + 2);
// 	else
// 		full_path = malloc(dir_len + cmd_len + 1);
// 	if (!full_path)
// 		return (NULL);
// 	if (dir_len > 0)
// 	{
// 		ft_memcpy(full_path, dir, dir_len);
// 		full_path[dir_len++] = '/';
// 	}
// 	ft_memcpy(full_path + dir_len, cmd, cmd_len);
// 	full_path[dir_len + cmd_len] = '\0';
// 	if (access(full_path, X_OK) == 0)
// 		return (full_path);
// 	free(full_path);
// 	return (NULL);
// }
