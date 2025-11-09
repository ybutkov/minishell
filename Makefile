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

CC = cc
# CFLAGS = -Wall -Wextra -Werror -g -fsanitize=address,undefined
CFLAGS = -Wall -Wextra -Werror -g -O0 $(INCLUDES)

LFLAGS = -L$(LIBFT_DIR) -l$(LIBFT_NAME)

C_PIPEX_FILES = \
	ast/ast_core.c \
	ast/ast_node_core.c \
	ast/ast_node_core_2.c \

C_FIlES = $(addprefix $(SRC)/, $(C_PIPEX_FILES))
C_OBJ_FIlES =	$(C_FIlES:%.c=$(BUILD_DIR)/%.o)

BUILD_DIRS := $(sort $(dir $(C_OBJ_FIlES)))

all : $(NAME)
bonus : all

${NAME} : $(LIBFT) $(BUILD_DIRS) $(C_OBJ_FIlES)
	@$(CC) $(CFLAGS) $(C_OBJ_FIlES) $(NAME_C) $(LFLAGS) -o $(NAME)
	@echo "Build OK"

clean :
	@rm -rf $(BUILD_DIR)
	@make -C $(LIBFT_DIR) clean $(PRINT_FLAG)
	@echo "clean OK"

fclean f: clean
	@rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean $(PRINT_FLAG)
	@echo "fclean OK"

re	: fclean all

$(BUILD_DIRS):
	@mkdir -p $@

$(BUILD_DIR)/%.o : %.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT) :
	@make -C $(LIBFT_DIR) all $(PRINT_FLAG)

.PHONY : all clean fclean re bonus

#valgrind --leak-check=full --show-leak-kinds=all --errors-for-leak-kinds=all -s ./pipex "infiles/basic.txt" "nonexistingcommand" "cat -e" "outfiles/outfile"
# minishell_test
