/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 18:54:03 by ybutkov           #+#    #+#             */
/*   Updated: 2025/07/10 20:21:22 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	find_next_word(char const *str, int *start, int *end, char ch)
{
	int	i;

	i = *start - 1;
	*start = -1;
	*end = -1;
	while (str[++i])
	{
		if (str[i] == ch)
		{
			if (*end == -1)
				continue ;
			else
				return (1);
		}
		if (*start == -1)
		{
			*start = i;
			*end = i;
		}
		else
			*end = i;
	}
	if (*start != -1 && *end != -1)
		return (1);
	return (0);
}

static char	*get_word(char const *src, int start, int end)
{
	char	*str;
	int		i;

	i = 0;
	str = malloc((end - start + 2) * sizeof(char));
	if (str == NULL)
		return (NULL);
	while (start <= end)
		str[i++] = src[start++];
	str[i] = '\0';
	return (str);
}

static char	**collect_words(char const *s, char c, char **arr_words)
{
	int		i;
	int		start;
	int		end;
	char	*word;

	start = 0;
	i = 0;
	while (find_next_word(s, &start, &end, c))
	{
		word = get_word(s, start, end);
		if (word == NULL)
		{
			while (i > 0)
				free(arr_words[--i]);
			free(arr_words);
			return (NULL);
		}
		arr_words[i++] = word;
		start = end + 1;
	}
	arr_words[i] = NULL;
	return (arr_words);
}

char	**ft_split(char const *s, char c)
{
	int		start;
	int		end;
	int		amount_words;
	char	**arr_words;

	start = 0;
	amount_words = 0;
	while (find_next_word(s, &start, &end, c))
	{
		amount_words++;
		start = end + 1;
	}
	arr_words = malloc((amount_words + 1) * sizeof(char *));
	if (arr_words == NULL)
		return (NULL);
	return (collect_words(s, c, arr_words));
}
