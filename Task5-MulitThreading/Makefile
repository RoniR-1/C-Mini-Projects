CC      = cc
CFLAGS  = -Wall -Wextra -Werror -g
NAME    = phils

SRCS    = main.c philisophers.c
OBJS    = $(SRCS:.c=.o)

all: $(NAME)


$(NAME): $(OBJS) $(MLX_LIB)
	$(CC) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re