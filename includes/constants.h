/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constants.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashadrin <ashadrin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 17:55:42 by ybutkov           #+#    #+#             */
/*   Updated: 2025/12/21 11:51:50 by ashadrin         ###   ########.fr       */
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
# define TERMINATED_BY_SIGINT 130
# define CMD_NOT_FOUND_MSG "command not found"
# define MSG_TOO_MANY_ARG "too many arguments"
# define MSG_INVALID_OPTION "invalid option"
# define EXIT_MSG_NUMBER_REQUIRED "numeric argument required"
# define EXIT_FAILURE_CREATE_PIPE 1
# define EXIT_FAILURE_CREATE_FORK 1

# define BUFFER_PATH 4096
# define HEREDOC_PREFIX_FILE "/tmp/heredoc_tmp_"
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
# define SYNTAX_ERROR_UNEXPECTED_TOKEN "syntax error near unexpected token `"
# define SYNTAX_ERROR_UNEXPECTED_EOF "unexpected end of file"
# define NOT_VALID_IDENTIFIER "not a valid identifier"
# define NO_SUCH_FILE_OR_DIR "No such file or directory"
# define COLON_SPACE ": "
# define NEW_LINE "\n"
# define BUFFER_PATH 4096

/*
1	Catchall for general errors	let "var1 = 1/0"
2	Misuse of shell builtins (according to Bash documentation)
126	Command invoked cannot execute	Permission problem or command is not an executable
127	"command not found"
128	Invalid argument to exit
128+n	Fatal error signal "n"
130	Script terminated by Control-C
255*	Exit status out of range
*/

#endif
