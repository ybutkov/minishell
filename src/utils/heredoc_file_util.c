/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_file_util.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 13:00:49 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/04 13:59:26 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <string.h>
#include "libft.h"
#include "constants.h"

int	get_file_n(int delta)
{
	static int	file_n;

	file_n += delta;
	return (file_n);
}

char	*get_tmp_file_name(int file_n)
{
	char	*file_name;
	char	*number;

	number = ft_itoa(file_n);
	if (number == NULL)
		return (NULL);
	file_name = ft_strjoin(HEREDOC_PREFIX_FILE, number);
	free(number);
	return (file_name);
}

void	clear_tmp_folder(int file_n)
{
	char	*tmp_path;

	while (file_n)
	{
		tmp_path = get_tmp_file_name(file_n);
		if (tmp_path)
		{
			unlink(tmp_path);
			free(tmp_path);
		}
		file_n--;
	}
}
