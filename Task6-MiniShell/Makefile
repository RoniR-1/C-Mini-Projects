CC          = cc
CFLAGS      = -Wall -Wextra -g -Iincludes -I../Task3-GetNext -I"../Task 1- Library"

NAME        = MingShell
TEST_BIN    = unit_tests

GNL_DIR     = ../Task3-GetNext
GNL_LIB     = $(GNL_DIR)/libgnl.a

LIBFT_DIR   = ../Task 1- Library
LIBFT_LIB   = $(LIBFT_DIR)/libft.a

SRC_DIR     = src
CORE_SRCS   = $(SRC_DIR)/loop_shell.c $(SRC_DIR)/ming_tokenizer.c $(SRC_DIR)/ming_parse.c $(SRC_DIR)/ming_execute.c $(SRC_DIR)/ming_calls.c
SRCS        = $(SRC_DIR)/main.c $(CORE_SRCS)
OBJS        = $(SRCS:.c=.o)

TEST_DIR	= tests
TEST_SRCS   = $(TEST_DIR)/test_main.c $(TEST_DIR)/tokenizer_test.c $(TEST_DIR)/parser_test.c $(CORE_SRCS)
TEST_OBJS   = $(TEST_SRCS:.c=.o)

all: $(NAME)

$(LIBFT_LIB):
	@$(MAKE) -C "$(LIBFT_DIR)" bonus

$(GNL_LIB):
	@$(MAKE) -C "$(GNL_DIR)"

$(NAME): $(OBJS) $(LIBFT_LIB) $(GNL_LIB)
	$(CC) $(CFLAGS) $(OBJS) "$(GNL_LIB)" "$(LIBFT_LIB)" -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TEST_BIN)
	@./$(TEST_BIN)

$(TEST_BIN): $(TEST_OBJS) $(LIBFT_LIB) $(GNL_LIB)
	$(CC) $(CFLAGS) $(TEST_OBJS) "$(GNL_LIB)" "$(LIBFT_LIB)" -o $(TEST_BIN)

clean:
	@rm -f $(OBJS) $(TEST_OBJS)
	@$(MAKE) -C "$(LIBFT_DIR)" clean
	@$(MAKE) -C "$(GNL_DIR)" clean

fclean: clean
	@rm -f $(NAME) $(TEST_BIN)
	@$(MAKE) -C "$(LIBFT_DIR)" fclean
	@$(MAKE) -C "$(GNL_DIR)" fclean

re: fclean all

.PHONY: all clean fclean re test