# **************************************************************************** #
# Makefile for libft (Parts 1, 2 & 3 - Bonus)                                  #
# **************************************************************************** #

# Program Name
NAME = libft.a

# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror
AR = ar rcs
RM = rm -f

# Part 1 - Libc functions
SRCS_P1 = ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c \
          ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c \
          ft_strlcpy.c ft_strlcat.c ft_toupper.c ft_tolower.c ft_strchr.c \
          ft_strrchr.c ft_strncmp.c ft_memchr.c ft_memcmp.c ft_strnstr.c \
          ft_atoi.c ft_calloc.c ft_strdup.c

# Part 2 - Additional functions
SRCS_P2 = ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c ft_itoa.c \
          ft_strmapi.c ft_striteri.c ft_putchar_fd.c ft_putstr_fd.c \
          ft_putendl_fd.c ft_putnbr_fd.c

# Part 3 - Bonus (Linked list functions)
SRCS_BONUS = ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c \
             ft_lstadd_back.c ft_lstdelone.c ft_lstclear.c ft_lstiter.c \
             ft_lstmap.c

# Combine all source files
SRCS = $(SRCS_P1) $(SRCS_P2)

# Bonus object files
OBJS_BONUS = $(SRCS_BONUS:.c=.o)

# Object files for mandatory parts
OBJS = $(SRCS:.c=.o)

# Colors for output (optional)
GREEN = \033[0;32m
RED = \033[0;31m
YELLOW = \033[1;33m
BLUE = \033[0;34m
CYAN = \033[0;36m
NC = \033[0m

# Default rule - builds the library without bonus
all: $(NAME)

# Main library creation (mandatory only)
$(NAME): $(OBJS)
	@$(AR) $(NAME) $(OBJS)
	@echo "$(GREEN)✓ Library $(NAME) created with $(words $(OBJS)) functions$(NC)"

# Compile .c files to .o files
%.o: %.c libft.h
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(YELLOW)Compiling:$(NC) $(notdir $<)"

# Bonus rule - builds with linked list functions
bonus: $(OBJS) $(OBJS_BONUS)
	@$(AR) $(NAME) $(OBJS) $(OBJS_BONUS)
	@echo "$(CYAN)✓ Bonus added! Library $(NAME) now has $(words $(OBJS) $(OBJS_BONUS)) functions$(NC)"

# Clean object files
clean:
	@$(RM) $(OBJS) $(OBJS_BONUS)
	@echo "$(RED)✗ Object files removed$(NC)"

# Full clean (objects + library)
fclean: clean
	@$(RM) $(NAME)
	@echo "$(RED)✗ Library $(NAME) removed$(NC)"

# Rebuild everything (mandatory only)
re: fclean all

# Rebuild with bonus
reb: fclean bonus

# Info about what's included
info:
	@echo "$(BLUE)=== libft Makefile Info ===$(NC)"
	@echo "$(BLUE)Part 1 (Libc):$(NC) $(words $(SRCS_P1)) functions"
	@echo "$(BLUE)Part 2 (Additional):$(NC) $(words $(SRCS_P2)) functions"
	@echo "$(BLUE)Part 3 (Bonus):$(NC) $(words $(SRCS_BONUS)) functions"
	@echo "$(BLUE)Total:$(NC) $(words $(SRCS) $(SRCS_BONUS)) functions"

# Phony targets (not actual files)
.PHONY: all bonus clean fclean re reb info