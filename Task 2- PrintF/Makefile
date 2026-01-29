NAME = libftprintf.a

# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror
AR = ar rcs
RM = rm -f

# Source files
SRCS = ft_printf.c 

# Alternatively, if you want all .c files in current directory:
# SRCS = $(wildcard *.c)

# Object files
OBJS = $(SRCS:.c=.o)

# Header files
HEADER = ft_printf.h

# Colors for output
GREEN = \033[0;32m
RED = \033[0;31m
YELLOW = \033[1;33m
BLUE = \033[0;34m
RESET = \033[0m

# Rules
all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(YELLOW)📦 Creating library $(NAME)...$(RESET)"
	@$(AR) $(NAME) $(OBJS)
	@echo "$(GREEN)✅ Library $(NAME) created successfully!$(RESET)"

%.o: %.c $(HEADER)
	@echo "$(BLUE)🔧 Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "$(RED)🧹 Cleaning object files...$(RESET)"
	@$(RM) $(OBJS)

fclean: clean
	@echo "$(RED)🗑️  Removing library...$(RESET)"
	@$(RM) $(NAME)

re: fclean all

# Bonus rule (if you decide to implement bonus)
bonus: all

# Test rule (optional)
test: all
	@echo "$(YELLOW)🧪 Running tests...$(RESET)"
	@$(CC) $(CFLAGS) main.c -L. -lftprintf -o test_printf
	@./test_printf

# Norm check rule (optional)
norm:
	@echo "$(YELLOW)📏 Checking norm...$(RESET)"
	@norminette $(SRCS) $(HEADER)

# Debug rule (optional)
debug: CFLAGS += -g
debug: re

# Memory check rule (optional)
memcheck: debug
	@$(CC) $(CFLAGS) main.c -L. -lftprintf -o test_printf
	@valgrind --leak-check=full --show-leak-kinds=all ./test_printf

.PHONY: all clean fclean re bonus test norm debug memcheck
