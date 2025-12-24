/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 16:36:45 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/24 23:13:37 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion_internal.h"
#include "libft.h"

static void	wild_with_path(char **pattern, char **path)
{
	char	*temp_pattern;
	char	*temp_path;
	char	*last_slash;
	int		len;

	last_slash = ft_strrchr(*pattern, '/');
	if (last_slash == NULL)
		return ((void)(*pattern = ft_strdup(*pattern)));
	len = last_slash - *pattern + 1;
	temp_path = malloc(sizeof(char) * (len + 1));
	if (!temp_path)
		return ;
	ft_strlcpy(temp_path, *pattern, len);
	temp_path[len] = '\0';
	len = ft_strlen(last_slash + 1);
	temp_pattern = malloc(sizeof(char) * (len + 1));
	if (!temp_pattern)
	{
		free(temp_path);
		return ;
	}
	ft_strcpy(temp_pattern, last_slash + 1);
	*pattern = temp_pattern;
	*path = temp_path;
}

static char	*build_full_name(char *path, char *file)
{
	char	*full_path;
	char	*temp;

	temp = ft_strjoin(path, "/");
	full_path = ft_strjoin(temp, file);
	free(temp);
	free(file);
	return (full_path);
}

static void	add_prefix(char *path, char **result)
{
	int	i;

	if (ft_strcmp(path, ".") == 0)
		return ;
	i = 0;
	while (result[i])
	{
		result[i] = build_full_name(path, result[i]);
		i++;
	}
}

static char	**empty_result(char	*path, char *pattern)
{
	char	**result;

	result = malloc(sizeof(char *) * 2);
	if (!result)
		return (NULL);
	result[0] = ft_strdup(pattern);
	result[1] = NULL;
	add_prefix(path, result);
	free(pattern);
	free(path);
	return (result);
}

char	**wildcard_expand(char *pattern)
{
	char	**result;
	DIR		*dir;
	int		size;
	char	*path;

	path = NULL;
	wild_with_path(&pattern, &path);
	if (path == NULL)
		path = ft_strdup(".");
	size = count_entries(pattern, path);
	if (size == 0)
		return (empty_result(path, pattern));
	dir = opendir(path);
	result = malloc(sizeof(char *) * (size + 1));
	if (!result)
		return (NULL);
	fill_matches(pattern, result, dir);
	sort_entries(result, size);
	closedir(dir);
	add_prefix(path, result);
	free(pattern);
	free(path);
	return (result);
}
