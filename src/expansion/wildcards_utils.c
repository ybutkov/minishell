/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 00:13:07 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/25 00:13:11 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion_internal.h"
#include "libft.h"

void	fill_matches(char *pattern, char **result, DIR *dir)
{
	struct dirent	*direntry;
	int				i;

	i = 0;
	direntry = readdir(dir);
	while (direntry)
	{
		if (pattern[0] != '.' && direntry->d_name[0] == '.')
		{
			direntry = readdir(dir);
			continue ;
		}
		if (suits_the_pattern(pattern, direntry->d_name, 0, 0))
		{
			result[i] = ft_strdup(direntry->d_name);
			i++;
		}
		direntry = readdir(dir);
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
	direntry = readdir(dir);
	while (direntry)
	{
		if (pattern[0] != '.' && direntry->d_name[0] == '.')
		{
			direntry = readdir(dir);
			continue ;
		}
		if (suits_the_pattern(pattern, direntry->d_name, 0, 0))
			count++;
		direntry = readdir(dir);
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
