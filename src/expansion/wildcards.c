/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 16:36:45 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/14 02:00:35 by ashadrin         ###   ########.fr       */
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

char	**wildcard_expand(t_piece *piece)
{
	char			**result;
	DIR				*dir;
	int				size;

	size = count_entries(piece);
	if (size == 0)
	{
		result = malloc(sizeof(char *) * 2);
		if (!result)
			return (NULL);
		result[0] = ft_strdup(piece->text);
		result[1] = NULL;
		return (result);
	}
	dir = opendir("."); // opendir returns a pointer to a DIR structure
	result = malloc(sizeof(char *) * size);
	if (!result)
		return (NULL);
	fill_matches(piece, result, dir);
	sort_entries(result, size);
	closedir(dir);
	return (result);
}

void	fill_matches(t_piece *piece, char **result, DIR *dir)
{
	struct dirent	*direntry;
	int				i;

	i = 0;
	direntry = readdir(dir);
	while (direntry)
	{
		if (piece->text[0] != '.' && direntry->d_name[0] == '.')
		{
			direntry = readdir(dir);
			continue ;
		}
		if (suits_the_pattern(piece->text, direntry->d_name, 0, 0))
		{
			result[i] = ft_strdup(direntry->d_name);
			i++;
		}
		direntry = readdir(dir);
	}
	result[i] = NULL;
}

int	count_entries(t_piece *piece)
{
	int				count;
	DIR				*dir;
	struct dirent	*direntry;

	count = 0;
	dir = opendir(".");
	if (!dir)
		return (0);
	while (1)
	{
		direntry = readdir(dir);
		if (!direntry)
			break ;
		if (piece->text[0] != '.' && direntry->d_name[0] == '.')
		{
			direntry = readdir(dir);
			continue ;
		}
		if (suits_the_pattern(piece->text, direntry->d_name, 0, 0))
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
		return (suits_the_pattern(pattern, filename, i, j + 1)
				|| suits_the_pattern(pattern, filename, i + 1, j));
	// if (pattern[i] != filename[j])
	// 	return (0);
	if (pattern[i] == filename[j])
		return (suits_the_pattern(pattern, filename, i + 1, j + 1));
	return (0);
}

// int	suits_the_pattern(char *pattern, char *filename)
// {
// 	int	i;
// 	int	j;
// 	int	len;

// 	i = 0;
// 	j = 0;
// 	len = ft_strlen(pattern);
// 	if (pattern[0] != '.' && filename[0] == '.')
// 		return (0);
// 	while (pattern[i] != '\0' && (pattern[i] == filename[j] || pattern[i] == '*'))
// 	{
// 		if (pattern[i] == '*')
// 		{
// 			if (pattern[i + 1] == '\0')
// 				return (1);
// 			while (filename[j] != pattern[i] && filename[j])
// 				j++;
// 			if (filename[j] == pattern[i])
// 				i++;
// 		}
// 		i++;
// 		j++;
// 	}
// 	if (i == len)
// 		return (1);
// 	return (0);
// }
