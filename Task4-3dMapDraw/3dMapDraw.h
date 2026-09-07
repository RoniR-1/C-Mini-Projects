#include "mlx_linux/mlx.h"

typedef struct	s_data {
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}	t_data;

void my_mlx_pixel_put(t_data* data, int x, int y, int colour);
