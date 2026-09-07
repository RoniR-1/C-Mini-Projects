#include "3dMapDraw.h"

int	main(void)
{
    void	*mlx;
    void	*mlx_win;
    t_data  img;

    /*
        Basically, call mlx_init to initialize and mlx_win to get the window
        Then mlx_new_image to get the image, not sure but I think it just handles things internally.
        Then call mlx_get_data_addr to gethe address, bpp, line length, endian
        Change the bytes from img.addre onwards to change the pixels.
        Once done, push it with mlx_put_image_to_window.
        Then mlx_loop to start
    */

    mlx = mlx_init();
    mlx_win = mlx_new_window(mlx, 1920, 1080, "Hello world!");

    img.img = mlx_new_image(mlx, 1920, 1080);
    img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length, &img.endian);


    // make a rectangle
    for (int y = 50; y <= 100; y += 50) {
        for (int x = 0; x <= 100; x++) {
            my_mlx_pixel_put(&img, x, y, 0x00FF0000);
        }
    }
    for (int x = 0; x <= 100; x += 100) {
        for (int y = 50; y <= 100; y++) {
            my_mlx_pixel_put(&img, x, y, 0x00FF0000);
        }
    }
    
    // attempted gradient
    for (int y = 200; y < 1000; y++) {
        for (int x = 200; x < 1500; x++) {
            my_mlx_pixel_put(&img, x, y, y+(1 << 15) + x*2);
        }
    }

    mlx_put_image_to_window(mlx, mlx_win, img.img, 0, 0);


    mlx_loop(mlx);
}

void my_mlx_pixel_put(t_data* data, int x, int y, int colour) {
    char* dst = data->addr + (data->line_length * y + x * (data->bits_per_pixel/8));
    *(unsigned int *) dst = colour;
}