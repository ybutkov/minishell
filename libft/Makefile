NAME = libft.a

CC = cc
CFLAGS = -Wall -Wextra -Werror -Iincludes

SRC = src
CORE = libft_core
UTILS = utils
GNL = get_next_line
STRUCTURES = structures
BUILD_DIR =	build

LIBFT_CORE_FILES = ft_isalpha.c \
		ft_isalnum.c \
		ft_isdigit.c \
		ft_isascii.c \
		ft_isprint.c \
		ft_strlen.c \
		ft_memset.c \
		ft_bzero.c \
		ft_memcpy.c \
		ft_memmove.c \
		ft_strlcpy.c \
		ft_strcpy.c \
		ft_strlcat.c \
		ft_toupper.c \
		ft_tolower.c \
		ft_str_capitalise.c \
		ft_strchr.c \
		ft_strrchr.c \
		ft_strcmp.c \
		ft_strncmp.c \
		ft_memchr.c \
		ft_memcmp.c \
		ft_strnstr.c \
		ft_atoi.c \
		ft_abs.c \
		ft_sign.c \
		ft_base_to_long.c \
		ft_calloc.c \
		ft_realloc.c \
		ft_strdup.c \
		ft_substr.c \
		ft_strjoin.c \
		ft_strappend.c \
		ft_strtrim.c \
		ft_split.c \
		ft_itoa.c \
		ft_double_to_str.c \
		ft_lltoa.c \
		ft_ultoa.c \
		ft_strmapi.c \
		ft_striteri.c \
		ft_putchar_fd.c \
		ft_putstr_fd.c \
		ft_putendl_fd.c \
		ft_putnbr_fd.c \
		ft_lstnew_bonus.c \
		ft_lstadd_front_bonus.c \
		ft_lstsize_bonus.c \
		ft_lstlast_bonus.c \
		ft_lstadd_back_bonus.c \
		ft_lstadd_back_array_bonus.c \
		ft_lstdelone_bonus.c \
		ft_lstclear_bonus.c \
		ft_lstiter_bonus.c \
		ft_lstmap_bonus.c \
		is_char_space.c

UTILS_FILES = compare_util.c \
		time_func.c \
		hash_code.c

GNL_FILES = get_next_line.c

STRUCTURES_FILES = base_stack/stack_base_basic.c \
		base_stack/stack_base_core.c \
		base_stack/stack_base_operations.c


C_FIlES = $(addprefix $(SRC)/$(CORE)/, $(LIBFT_CORE_FILES))
C_FIlES += $(addprefix $(SRC)/$(UTILS)/, $(UTILS_FILES))
C_FIlES += $(addprefix $(SRC)/$(GNL)/, $(GNL_FILES))
C_FIlES += $(addprefix $(SRC)/$(STRUCTURES)/, $(STRUCTURES_FILES))
C_OBJ_FIlES = $(C_FIlES:%.c=$(BUILD_DIR)/%.o)

BUILD_DIRS := $(sort $(dir $(C_OBJ_FIlES)))

all : $(NAME)

${NAME} : $(BUILD_DIRS) $(C_OBJ_FIlES)
	ar rcs $(NAME) $(C_OBJ_FIlES)

clean :
	rm -rf $(BUILD_DIR)

fclean f: clean
	rm -f $(NAME)

re	: fclean all

$(BUILD_DIRS):
	mkdir -p $@

$(BUILD_DIR)/%.o : %.c | $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY : all clean fclean f re

# valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes ./pipex infile "ls -la" "wc -l" outfile
