CC      = cc
CFLAGS  = -Wall -Wextra -Werror -g
NAME    = 3dMap

SRCS    = main.c fdfParser.c fdf_utils.c
OBJS    = $(SRCS:.c=.o)

MLX_DIR = mlx_linux
MLX_LIB = $(MLX_DIR)/libmlx_Linux.a	
GET_NEXT = ../Task3-GetNext/getNextLine.a

all: $(NAME)
$(GET_NEXT):
	@make -C $(../Task3-GetNext)

$(MLX_LIB):
	@make -C $(MLX_DIR)

$(NAME): $(OBJS) $(MLX_LIB)
	$(CC) $(OBJS) $(GET_NEXT) -L$(MLX_DIR) -lmlx_Linux -L/usr/lib -I$(MLX_DIR) -lXext -lX11 -lm -lz -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -I/usr/include -I$(MLX_DIR) -O3 -c $< -o $@

clean:
	rm -f $(OBJS)
	@make -C $(MLX_DIR) clean

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re