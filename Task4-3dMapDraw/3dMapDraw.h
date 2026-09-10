#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <fcntl.h>
#include <unistd.h>
#include "mlx_linux/mlx.h"
#include "../Task3-GetNext/getNext.h"

#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

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


typedef struct s_grid {
	int		rowLen;
	int		colLen;
	int**	grid;
	unsigned int color;
	int angleX;
	int angleY;
	int angleZ;
}	t_grid;


typedef struct s_mlx {
	t_data	*data;
    t_vars	*vars;
	t_grid	*grid;
	int		width;
	int		height;
}	t_mlx;



int drawMap(char* file);
int* parseCords(char* s, int* size);
void my_mlx_pixel_put(t_data *data, int x, int y, unsigned int color);
void drawBetween2Points(int x1, int y1, int x2, int y2, unsigned int color, t_data* data);
void drawGridIso(t_grid* grid_data, t_data* data);