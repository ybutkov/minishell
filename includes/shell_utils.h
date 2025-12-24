/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 18:18:35 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/24 02:49:48 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_UTILS_H
# define SHELL_UTILS_H

void	free_str_array(char **arr);
void	clear_tmp_folder(int file_n);
char	*get_tmp_file_name(int file_n);
int		get_file_n(int delta);
void	apply_redirects(t_list *redir, t_shell *shell);
void	apply_cmd_redirects(t_cmd *cmd, t_shell *shell);

#endif
