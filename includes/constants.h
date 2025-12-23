/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constants.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybutkov <ybutkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 17:55:42 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/23 23:24:15 by ybutkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONSTANTS_H
# define CONSTANTS_H

# define HANDLE_ERROR_NULL NULL
# define HANDLE_ERROR_MINUS_ONE -1
# define OK 1
# define NO 0
# define ERROR 0
# define EXIT_MISUSE 2
# define EXIT_CMD_CANNOT_EXEC 126
# define EXIT_CMD_NOT_FOUND 127
# define EXIT_TERMINATED_BY_SIGINT 130
# define CMD_NOT_FOUND_MSG "command not found"
# define MSG_TOO_MANY_ARG "too many arguments"
# define MSG_INVALID_OPTION "invalid option"
# define EXIT_MSG_NUMBER_REQUIRED "numeric argument required"
# define EXIT_FAILURE_CREATE_PIPE 1
# define EXIT_FAILURE_CREATE_FORK 1

# define BUFFER_PATH 4096
# define HEREDOC_PREFIX_FILE "/tmp/heredoc_tmp_"
# define HEREDOC_PROMT "heredoc> "
# define HOME "HOME"
# define SHLVL "SHLVL"
# define PATH "PATH"
# define MSG_EXIT "exit"
# define MSG_EXIT_2_COLON "exit: "
# define CD_MSG_2_COLON "cd: "
# define MSG_CD "cd"
# define MSG_EXPORT_2_COLON "export: -x"
# define MSG_DECLARE_X "declare -x "
# define MSG_EXIT_NL "exit\n"
# define SHELL_ERROR_PREFIX "minishell: "
# define SYNTAX_ERROR "syntax error"
# define MALLOC_ERROR "malloc error"
# define FORK_ERROR "fork error"
# define SYNTAX_ERROR_UNEXPECTED_TOKEN "syntax error near unexpected token `"
# define HEREDOC_DELIMITED_EOF "here-document delimited by end-of-file (wanted '"
# define SYNTAX_ERROR_UNEXPECTED_EOF "unexpected end of file"
# define MSG_IS_DIRECTORY "is a directory"
# define NOT_VALID_IDENTIFIER "not a valid identifier"
# define NO_SUCH_FILE_OR_DIR "No such file or directory"
# define COLON_SPACE ": "
# define NEW_LINE "\n"
# define BUFFER_PATH 4096

#endif
