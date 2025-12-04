/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constants.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 17:55:42 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/06 15:39:04 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONSTANTS_H
# define CONSTANTS_H

# define OK 1
# define ERROR 0
# define EXIT_CMD_NOT_FOUND 127
# define CMD_NOT_FOUND_MSG "command not found"
# define EXIT_FAILURE_CREATE_PIPE 1
# define EXIT_FAILURE_CREATE_FORK 1

# define BUFFER_PATH 4096
# define HEREDOC_PREFIX_FILE "/tmp/heredoc_tmp_"
# define SHELL_ERROR_PREFIX "minishell: "
# define COLON_SPACE ": "
# define NEW_LINE "/n"

#endif
