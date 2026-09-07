#include "3dMapDraw.h"

int	main(void)
{
    void	*mlx;
    void	*mlx_win;
    t_data  img;

    mlx = mlx_init();
    mlx_win = mlx_new_window(mlx, 1920, 1080, "Hello world!");

    img.img = mlx_new_image(mlx, 1920, 1080);
    img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length, &img.endian);

    for (int y = 1; y < 1080; y *= 3) {
        for (int x = 1; x < 1920; x *= 2) {
            my_mlx_pixel_put(&img, x-1, y, 0x00220000);
            my_mlx_pixel_put(&img, x-1, y-1, 0x00F00F00);
            my_mlx_pixel_put(&img, x, y-1, 0x0010000F);
            my_mlx_pixel_put(&img, x, y, 0xF0F00F00);
        }
    }

    mlx_put_image_to_window(mlx, mlx_win, img.img, 0, 0);


    mlx_loop(mlx);
}

void my_mlx_pixel_put(t_data* data, int x, int y, int colour) {
    char* dst = data->addr + (data->line_length * y + x * (data->bits_per_pixel/8));
    *(unsigned int *) dst = colour;
}