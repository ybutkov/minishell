/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_execute_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 23:28:32 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/02 23:29:13 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	dup2_and_close(int oldfd, int newfd)
{
	dup2(oldfd, newfd);
	close(oldfd);
}

