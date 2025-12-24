NAME = minishell
NAME_C = minishell.c

SRC = src
BUILD_DIR =	build

LIBFT_DIR = libft
LIBFT_NAME = ft
LIBFT = $(LIBFT_DIR)/libft.a

INCLUDES = -Iincludes
INCLUDES += -I$(LIBFT_DIR)/includes

PRINT_FLAG = -s --no-print-directory

DEFAULT_SILENT ?= 1
ifneq ($(filter s,$(MAKECMDGOALS)),)
SILENT := 0
else
SILENT ?= $(DEFAULT_SILENT)
endif
ifeq ($(SILENT),1)
MAKEFLAGS += -s --no-print-directory
endif

CC = cc
# CFLAGS = -Wall -Wextra -Werror -g -fsanitize=address,undefined
CFLAGS = -Wall -Wextra -Werror -g -O0 $(INCLUDES)
LFLAGS = -L$(LIBFT_DIR) -l$(LIBFT_NAME) -lreadline -lncurses
#two last flags are for compiling an external library for readline
SAN_ASAN = -fsanitize=address,undefined -fno-omit-frame-pointer

C_SHELL_FILES = \
	ast/ast_core.c \
	ast/ast_node_core.c \
	ast/ast_node_core_2.c

C_SHELL_FILES += \
 	shell/shell_core.c \
	shell/shell_build.c \
	shell/shell_build_subshell.c \
	shell/shell_build_redirs.c \
	shell/shell_build_leaves.c \
	shell/shell_execute_tree.c \
	shell/shell_execute_pipe.c \
	shell/shell_execute_heredoc_tree.c \
	shell/shell_execute_heredoc_expand.c \
	shell/shell_execute_heredoc_input.c \
	shell/shell_execute_cmd.c \
	shell/shell_execute_builtin_type.c \
	shell/shell_node.c \
	shell/redirect.c \
	shell/commands.c \
	shell/contex.c \
	shell/enams.c \
	shell/files_operations.c \
	shell/shell_utils.c

C_SHELL_FILES += \
	error/output_error.c \
	utils/free_str_array.c \
	utils/heredoc_file_util.c \
	utils/list_to_array.c \
	printer/print_shell.c \
	printer/print_shell2.c \
	printer/print_tokens.c

C_SHELL_FILES += \
	lexer/lexer.c \
	lexer/mixed_value_assign.c \
	lexer/spaces_quotes.c \
	lexer/symbol_identifier.c \
	lexer/tokenizer.c \
	lexer/tokenizer2.c \
	lexer/utils.c \
	lexer/read_line.c \
	lexer/preprocessor.c \
	lexer/freeing.c \
	lexer/tokens.c \
	lexer/assigning_pieces.c \
	lexer/extras_decide.c \

C_SHELL_FILES += \
	built_ins/cd.c \
	built_ins/echo.c \
	built_ins/env.c \
	built_ins/exit.c \
	built_ins/export.c \
	built_ins/export1.c \
	built_ins/export2.c \
	built_ins/pwd.c \
	built_ins/unset.c \
	built_ins/bi_wrapper_1.c \
	built_ins/bi_wrapper_2.c 

C_SHELL_FILES += \
	envp/envp_copy.c \
	envp/copy_to_array.c \
	envp/envp_list_init.c

C_SHELL_FILES += \
	expansion/expansion.c \
	expansion/wildcards.c \
	expansion/collecting.c \
 	parcer/get_cmd_path.c \
	parcer/validator_checker.c \
	parcer/validator_bools.c \
	parcer/validator.c \
	signals/signals.c \
	signals/handle_and_disable.c

C_FIlES = $(addprefix $(SRC)/, $(C_SHELL_FILES))
C_OBJ_FIlES =	$(C_FIlES:%.c=$(BUILD_DIR)/%.o)

BUILD_DIRS := $(sort $(dir $(C_OBJ_FIlES)))

all : $(NAME)
bonus : all

${NAME} : $(LIBFT) $(BUILD_DIRS) $(C_OBJ_FIlES)
	$(CC) $(CFLAGS) $(C_OBJ_FIlES) $(NAME_C) $(LFLAGS) -o $(NAME)
	@echo "Build OK"

clean :
	@rm -rf $(BUILD_DIR)
	@make -C $(LIBFT_DIR) clean $(PRINT_FLAG)
	@echo "clean OK"

fclean f: clean
	@rm -f $(NAME)
	@rm -f $(TEST_RUNNER)
	@make -C $(LIBFT_DIR) fclean $(PRINT_FLAG)
	@echo "fclean OK"

re	: fclean all
s : all
c : re
	cp ./minishell ${HOME}/Documents/tmp_folder/minishell_for_test
	cp ./minishell ${HOME}/42_minishell_tester

# adds AddressSanitizer and UndefinedBehaviorSanitizer flags
asan: CFLAGS += $(SAN_ASAN)
asan: re

$(BUILD_DIRS):
	@mkdir -p $@

$(BUILD_DIR)/%.o : %.c
# ???
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT) :
	@make -C $(LIBFT_DIR) all $(PRINT_FLAG)

.PHONY : all clean fclean re bonus s

.PHONY: v

v: re
	valgrind --leak-check=full  --show-leak-kinds=all --track-fds=yes \
		--suppressions=tools/sub.sub  ./minishell
# 		--track-origins=yes --trace-children=yes --suppressions=tools/sub.sub  ./minishell

#valgrind --leak-check=full --show-leak-kinds=all --errors-for-leak-kinds=all -s ./minishell

