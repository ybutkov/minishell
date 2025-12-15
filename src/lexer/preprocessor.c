/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preprocessor.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 22:08:41 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/15 16:07:10 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <string.h>

char	*preprocessing(char *str)
{
	char	*res;
	int		i;
	int		j;
	int		res_len;
	int		single_opened;
	int		double_opened;
	int		delta_size;

	res_len = ft_strlen(str);
	res = (char *)malloc(sizeof(char) * (res_len + 1));
	if (res == NULL)
		return (free(str), NULL);
	memset(res, 0, (res_len + 1));
	single_opened = 0;
	double_opened = 0;
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '\'')
		{
			if (single_opened == 0)
				single_opened = 1;
			else
				single_opened = 0;
			res[j++] = str[i++];
			continue ;
		}
		if (str[i] == '\"')
		{
			if (double_opened == 0)
				double_opened = 1;
			else
				double_opened = 0;
			res[j++] = str[i++];
			continue ;
		}
		if (str[i] == '\\' && single_opened == 0)
		{
			if (double_opened == 1)
				delta_size = 3;
			else
				delta_size = 1;
			res = (char *)ft_realloc(res, res_len, res_len + delta_size + 1);
			res_len += delta_size;
			if (res == NULL)
				return (free(str), NULL);
			if (double_opened == 1)
				res[j++] = '\"';
			res[j++] = '\'';
			i++;
			res[j++] = str[i++];
			res[j++] = '\'';
			if (double_opened == 1)
				res[j++] = '\"';
		}
		else
			res[j++] = str[i++];
	}
	return (res);
}

