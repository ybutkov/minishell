/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 20:05:36 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/16 23:14:58 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include <stdlib.h>
# include "../libft/includes/libft.h"

void	free_str_array(char **arr);
void	**list_to_array_generic(t_list *lst, void *(*convert)(void *));
char	**list_to_array(t_list *lst);
void	empty_func(void *p);

#endif
