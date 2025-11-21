/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compare_util.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 13:58:36 by ybutkov           #+#    #+#             */
/*   Updated: 2025/09/28 15:31:18 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

int	between(long long value, long long left, long long right)
{
	if (value >= left && value <= right)
		return (1);
	return (0);
}

int	in_set(char ch, char *set)
{
	while (*set)
	{
		if (ch == *set)
			return (1);
		set = set + 1;
	}
	return (0);
}
