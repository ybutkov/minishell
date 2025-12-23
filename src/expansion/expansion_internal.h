/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_internal.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 12:38:12 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/23 17:10:11 by ashadrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSION_INTERNAL_H
# define EXPANSION_INTERNAL_H

# include "parsing.h"
# include <dirent.h>
# include "envp_copy.h"

//wildcards
char	**wildcard_expand(t_piece *piece);
void	fill_matches(t_piece *piece, char **result, DIR *dir);
int		count_entries(t_piece *piece);
void	sort_entries(char **result, int size);
int		suits_the_pattern(char *pattern, char *filename, int i, int j);

#endif