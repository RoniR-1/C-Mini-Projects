#include "mlx_linux/mlx.h"
#include "../Task3-GetNext/getNext.h"

typedef struct	s_data {
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}	t_data;

typedef struct s_vars {
	void	*mlx;
    void	*mlx_win;
}	t_vars;

int drawMap(char* file);
int* parseCords(char* s, int* size);