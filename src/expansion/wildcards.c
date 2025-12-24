/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 16:36:45 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/24 15:01:29 by ashadrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// • Wildcards * should work for the current working directory.
//shell globbing -- shell's filename expansion
// If the pattern starts with . (e.g., .*, .git*), then allow matches whose names start with ..
// If the pattern does not start with ., skip any directory entry whose name starts with .
//(both . and .., and all dotfiles). This mirrors typical shell globbing.
// So: check pattern[0] == '.' → allow hidden; else if d_name[0] == '.' → skip.

//bash sorts results alphabetically, but readddir() reads them in filesystem order

#include "expansion_internal.h"

char	**wildcard_expand(char *pattern)
{
	char			**result;
	DIR				*dir;
	int				size;
	char			*path;

	path = ".";
	if (ft_strchr(pattern, '/'))
		wild_with_path(&pattern, &path);
	size = count_entries(pattern, path);
	// printf("wildcard_expand: pattern='%s', size=%d\n", piece->text, size);
	if (size == 0)
	{
		result = malloc(sizeof(char *) * 2);
		if (!result)
			return (NULL);
		result[0] = ft_strdup(pattern);
		result[1] = NULL;
		return (result);
	}
	dir = opendir(path); // opendir returns a pointer to a DIR structure
	result = malloc(sizeof(char *) * (size + 1));
	if (!result)
		return (NULL);
	fill_matches(pattern, result, dir);
	sort_entries(result, size);
	closedir(dir);
	if (pattern)
		free(pattern);
	if (path)
		free(path);
	return (result);
}

void	wild_with_path(char **pattern, char **path)
{
	char	*temp_pattern;
	char	*temp_path;
	char	*last_slash;
	int		len;

	last_slash = ft_strrchr(pattern, '/');
	if (last_slash == NULL)
		return;
	len = last_slash - pattern;
	temp_path = malloc(sizeof(char) * len + 1);
	if (!temp_path)
		return ;
	ft_strncpy(temp_path, pattern, len);
	temp_path[len] = '\0';
	len = ft_strlen(last_slash + 1);
	temp_pattern = malloc(sizeof(char) * len + 1);
	if (!temp_pattern)
	{
		free(temp_path);
		return ;
	}
	ft_strcpy(temp_pattern, last_slash + 1);
	*pattern = temp_pattern;
	*path = temp_path;
}

void	fill_matches(char *pattern, char **result, DIR *dir)
{
	struct dirent	*direntry;
	int				i;

	i = 0;
	while ((direntry = readdir(dir)))
{
    // printf("Checking: %s against pattern %s\n", direntry->d_name, piece->text);
    if (pattern[0] != '.' && direntry->d_name[0] == '.')
        continue;
    if (suits_the_pattern(pattern, direntry->d_name, 0, 0))
    {
        // printf("MATCHED: %s\n", direntry->d_name);
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
        continue;
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
		while (j < size - 1 - i) // -i because i elements at the end are already definitely sorted
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
	// if (pattern[i] != filename[j])
	// 	return (0);
	if (pattern[i] == filename[j])
		return (suits_the_pattern(pattern, filename, i + 1, j + 1));
	return (0);
}
