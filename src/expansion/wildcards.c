/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 16:36:45 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/24 18:23:10 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion_internal.h"
#include "libft.h"

void	wild_with_path(char **pattern, char **path)
{
	char	*temp_pattern;
	char	*temp_path;
	char	*last_slash;
	int		len;

	last_slash = ft_strrchr(*pattern, '/');
	if (last_slash == NULL)
	// {
	// 	*pattern = ft_strdup(*pattern);
	// 	return ;
	// }
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

char	*build_full_name(char *path, char *file)
{
	char	*full_path;
	char	*temp;

	temp = ft_strjoin(path, "/");
	full_path = ft_strjoin(temp, file);
	free(temp);
	free(file);
	return (full_path);
}

void	add_prefix(char *path, char **result)
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

char	**empty_result(char	*path, char *pattern)
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

void	fill_matches(char *pattern, char **result, DIR *dir)
{
	struct dirent	*direntry;
	int				i;

	i = 0;
	while ((direntry = readdir(dir)))
	{
		if (pattern[0] != '.' && direntry->d_name[0] == '.')
			continue ;
		if (suits_the_pattern(pattern, direntry->d_name, 0, 0))
		{
			result[i] = ft_strdup(direntry->d_name);
			i++;
		}
	}
	result[i] = NULL;
}

int	count_entries(char *pattern, char *path)
{
	int				count;
	DIR				*dir;
	struct dirent	*direntry;

	count = 0;
	dir = opendir(path);
	if (!dir)
		return (0);
	while ((direntry = readdir(dir)))
	{
		if (pattern[0] != '.' && direntry->d_name[0] == '.')
			continue ;
		if (suits_the_pattern(pattern, direntry->d_name, 0, 0))
			count++;
	}
	closedir(dir);
	return (count);
}

void	sort_entries(char **result, int size)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while (i < size)
	{
		j = 0;
		while (j < size - 1 - i)
		{
			if (ft_strcmp(result[j], result[j + 1]) > 0)
			{
				temp = result[j];
				result[j] = result[j + 1];
				result[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}

int	suits_the_pattern(char *pattern, char *filename, int i, int j)
{
	if (pattern[i] == '\0' && filename[j] == '\0')
		return (1);
	if (pattern[i] == '*')
	{
		if (filename[j] == '\0')
			return (suits_the_pattern(pattern, filename, i + 1, j));
		return (suits_the_pattern(pattern, filename, i, j + 1)
			|| suits_the_pattern(pattern, filename, i + 1, j));
	}
	if (pattern[i] == filename[j])
		return (suits_the_pattern(pattern, filename, i + 1, j + 1));
	return (0);
}
