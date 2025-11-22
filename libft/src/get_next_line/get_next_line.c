/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 17:40:04 by ybutkov           #+#    #+#             */
/*   Updated: 2025/11/17 14:54:24 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "libft.h"
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static char	*fill_str_from_buffer(char *cur_str, int *total_len, char *buffer,
				int amount_bytes);
static char	*read_from_fd(int fd, char *res, int total_len, char *buffer);

char	*get_next_line(int fd)
{
	static char	buffer[BUFFER_SIZE];
	int			total_len;
	char		*res;

	if (fd < 0 || fd >= FD_MAX_SIZE)
		return (NULL);
	res = NULL;
	total_len = 0;
	if (buffer[0])
		res = fill_str_from_buffer(NULL, &total_len, buffer, BUFFER_SIZE);
	if (buffer[0] != '\0' && res == NULL)
		return (NULL);
	if (res && res[total_len - 1] == '\n')
		return (res[total_len] = '\0', res);
	res = read_from_fd(fd, res, total_len, buffer);
	return (res);
}

static char	*fill_str_from_buffer(char *cur_str, int *total_len, char *buffer,
		int amount_bytes)
{
	int		len;
	char	*res;

	len = 0;
	res = NULL;
	while (len < amount_bytes && buffer[len] && buffer[len] != '\n')
		len++;
	if (len < amount_bytes && buffer[len] == '\n')
		len += 1;
	if (len == 0)
		return (cur_str);
	res = malloc((*total_len + len + 1) * sizeof(char));
	if (res == NULL)
		return (free(cur_str), NULL);
	if (cur_str)
	{
		ft_memmove(res, cur_str, *total_len);
		free(cur_str);
	}
	ft_memmove(res + *total_len, buffer, len);
	ft_memmove(buffer, buffer + len, amount_bytes - len);
	ft_memset(buffer + amount_bytes - len, '\0', len);
	*total_len += len;
	return (res);
}

static char	*read_from_fd(int fd, char *res, int total_len, char *buffer)
{
	ssize_t	read_bytes;

	read_bytes = 1;
	while (read_bytes > 0)
	{
		read_bytes = read(fd, buffer, BUFFER_SIZE);
		if (read_bytes < 0)
		{
			if (res)
				free(res);
			return (NULL);
		}
		else if (read_bytes == 0)
			break ;
		res = fill_str_from_buffer(res, &total_len, buffer, read_bytes);
		if (!res)
			return (NULL);
		if (res[total_len - 1] == '\n')
			break ;
	}
	if (res)
		res[total_len] = '\0';
	return (res);
}

// int main()
// {
// 	char *f = "file.txt";
// 	int fd = open(f, O_RDONLY);

// 	char *str = get_next_line(fd);
// 	printf("%s", str);
// 	if (str)
// 		free(str);
// 	str = get_next_line(fd);
// 	printf("%s", str);
// 	if (str)
// 		free(str);
// 	close(fd);
// 	return 0;
// }
