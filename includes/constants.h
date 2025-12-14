/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constants.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 17:55:42 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/14 17:50:51 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONSTANTS_H
# define CONSTANTS_H

# define HANDLE_ERROR_NULL NULL
# define HANDLE_ERROR_MINUS_ONE -1
# define OK 1
# define ERROR 0
# define EXIT_CMD_NOT_FOUND 127
# define CMD_NOT_FOUND_MSG "command not found"
# define EXIT_FAILURE_CREATE_PIPE 1
# define EXIT_FAILURE_CREATE_FORK 1

# define BUFFER_PATH 4096
# define HEREDOC_PREFIX_FILE "/tmp/heredoc_tmp_"
# define HOME "HOME"
# define SHELL_ERROR_PREFIX "minishell: "
# define SYNTAX_ERROR "syntax error"
# define MALLOC_ERROR "malloc error"
# define SYNTAX_ERROR_UNEXPECTED_TOKEN "syntax error near unexpected token `"
# define SYNTAX_ERROR_UNEXPECTED_EOF "unexpected end of file"
# define NOT_VALID_IDENTIFIER "not a valid identifier"
# define COLON_SPACE ": "
# define NEW_LINE "\n"
# define BUFFER_PATH 4096

#endif
