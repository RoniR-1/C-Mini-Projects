CC      = cc
CFLAGS  = -Wall -Wextra -Werror
NAME    = 3dMap

SRCS    = main.c
OBJS    = $(SRCS:.c=.o)

MLX_DIR = mlx_linux
MLX_LIB = $(MLX_DIR)/libmlx_Linux.a	

all: $(NAME)

$(MLX_LIB):
	@make -C $(MLX_DIR)

$(NAME): $(OBJS) $(MLX_LIB)
	$(CC) $(OBJS) -L$(MLX_DIR) -lmlx_Linux -L/usr/lib -I$(MLX_DIR) -lXext -lX11 -lm -lz -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -I/usr/include -I$(MLX_DIR) -O3 -c $< -o $@

clean:
	rm -f $(OBJS)
	@make -C $(MLX_DIR) clean

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re