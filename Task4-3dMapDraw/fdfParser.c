#include <X11/keysym.h>
#include "3dMapDraw.h"

int	closeWindow(t_vars *vars) {
    mlx_destroy_window(vars->mlx, vars->mlx_win);
    exit(0);
}
/*
    Sets the image to black
*/
void clean_window(t_data* data, int width, int height) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            *(unsigned int*)(data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8))) = 0x0;
        }
    }
}

void handle_rotations_by_user(int keycode, t_mlx* mlx_data) {
    if (keycode == XK_comma || keycode == XK_period) {
        if (keycode == XK_Right) mlx_data->grid->angleY -= 10;
        else mlx_data->grid->angleY += 10;
        if (mlx_data->grid->angleY >= 360) mlx_data->grid->angleY -= 360;
        if (mlx_data->grid->angleY < 0) mlx_data->grid->angleY += 360;
    }
    else if (keycode == XK_Left || keycode == XK_Right) {
        if (keycode == XK_Right) mlx_data->grid->angleZ -= 10;
        else mlx_data->grid->angleZ += 10;
        if (mlx_data->grid->angleZ >= 360) mlx_data->grid->angleZ -= 360;
        if (mlx_data->grid->angleZ < 0) mlx_data->grid->angleZ += 360;
    }
    else {
        if (keycode == XK_Down) mlx_data->grid->angleX -= 10;
        else mlx_data->grid->angleX += 10;
        if (mlx_data->grid->angleX >= 360) mlx_data->grid->angleX -= 360;
        if (mlx_data->grid->angleX < 0) mlx_data->grid->angleX += 360;
    }
    
    clean_window(mlx_data->data, mlx_data->width, mlx_data->height);

    drawGridIso(mlx_data->grid, mlx_data->data);
    mlx_put_image_to_window(mlx_data->vars->mlx, mlx_data->vars->mlx_win, mlx_data->data->img, 0, 0);
}
int	handleKey(int keycode, t_mlx* mlx_data) {
    if (keycode == 27 || keycode == XK_Escape) {
        return closeWindow(mlx_data->vars);
    }
    if (keycode == XK_Left || keycode == XK_Right || keycode == XK_Up || keycode == XK_Down || keycode == XK_comma || keycode == XK_period) {
        handle_rotations_by_user(keycode, mlx_data);
        printf("rawr\n");
    }
	return 0;
}

int drawMap(char* file) {
    // inits
    t_data data;
    t_vars vars;
    t_mlx mlx_data;
    t_grid grid_data;
    char* line;

    mlx_data.data = &data;
    mlx_data.vars = &vars;
    mlx_data.grid = &grid_data;
    mlx_data.width = 1920;
    mlx_data.height = 1080;

    grid_data.color = 0x00FF0000;
    grid_data.angleX = 0;
    grid_data.angleY = 0;
    grid_data.angleZ = 0;

    vars.mlx = mlx_init();
    vars.mlx_win = mlx_new_window(vars.mlx, mlx_data.width, mlx_data.height, "GRID");
    data.img = mlx_new_image(vars.mlx, mlx_data.width, mlx_data.height);
    data.addr = mlx_get_data_addr(data.img, &data.bits_per_pixel, &data.line_length, &data.endian);

    int fd = open(file, O_RDONLY);
    if (fd < 0) {
        perror("Error opening file");
        return -1;
    }
    // read the grid line by line and storing it
    line = get_next_line(fd);
    int counter = 0;
    int len = 0;
    while(line[counter]) {
        if (line[counter] != ' ' && line[counter] != '\n' && (counter == 0 || line[counter-1] == ' ')) {
            len++;
        }
        counter++;
    }
    grid_data.grid = malloc(sizeof(int*) * len);
    if (grid_data.grid == NULL) {
        printf("Error allocations on grid\n");
        return -1;
    }
    grid_data.rowLen = 0; grid_data.colLen = 0;
    while(line != NULL) {
        printf("inLine\n");
        grid_data.grid[grid_data.colLen] = parseCords(line, &grid_data.rowLen);
        line = get_next_line(fd);
        grid_data.colLen++;
    }
    close(fd); // close file
    // x = (x-y) * cos(30 deg)
    // y = (x+y) * sin(30 deg) - z
    // now having it integer grid, apply the formula on the fly and draw
    drawGridIso(&grid_data, &data);
    mlx_put_image_to_window(vars.mlx, vars.mlx_win, data.img, 0, 0);



    // add hooks now to exit, rotate with keys

    mlx_hook(vars.mlx_win, 17, 1L << 0, closeWindow, &vars); // handle closing
    mlx_hook(vars.mlx_win, 2, 0, handleKey, &mlx_data); // handle key
    mlx_loop(vars.mlx);
    return 0;
}