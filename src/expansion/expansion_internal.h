/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_internal.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 12:38:12 by ashadrin          #+#    #+#             */
/*   Updated: 2025/12/23 20:41:25 by ashadrin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSION_INTERNAL_H
# define EXPANSION_INTERNAL_H

# include "parsing.h"
# include <dirent.h>
# include "envp_copy.h"

//wildcards
char	**wildcard_expand(char *pattern);
void	fill_matches(char *pattern, char **result, DIR *dir);
int		count_entries(char *pattern);
void	sort_entries(char **result, int size);
int		suits_the_pattern(char *pattern, char *filename, int i, int j);

#endif