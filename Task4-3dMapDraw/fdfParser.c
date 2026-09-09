#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <fcntl.h>
#include <unistd.h>
#include "3dMapDraw.h"


/*
    Parses string of ints with space ' ' seperating them and returns an array of ints
*/
int* parseCords(char* s, int* size) {
    int len = 0;
    int counter = 0;
    while(s[counter]) {
        if (s[counter] != ' ' && s[counter] != '\n' && (counter == 0 || s[counter-1] == ' ')) {
            len++;
        }
        counter++;
    }
    *size = len;
    int* result = malloc(sizeof(int) * len);

    len = 0;
    counter = 0;
    while(s[counter]) {
        if (s[counter] != ' ' && s[counter] != '\n') {
            int x = 0;
            int isNegative = -1;
            while(s[counter] && s[counter] != ' ' && s[counter] != '\n') {
                if (s[counter] == '-') {
                    isNegative = 1;
                }
                else x = x * 10 + s[counter] - '0';
                counter++;
            }
            if (isNegative == 1) x *= -1;
            result[len] = x;
            len++;
        }
        else counter++;
    }
    return result;
}

void my_mlx_pixel_put(t_data *data, int x, int y, unsigned int color) {
    printf("%d-%d\n", x, y);
	char* dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}
// Uses Bresenham's to draw a line between 2 points in a grid, vertical
void drawBetween2PointsVertic(int x1, int y1, int x2, int y2, unsigned int color, t_data data) {
    if (y1 > y2) {
        int temp = x1;
        x1 = x2;
        x2 = temp;
        temp = y1;
        y1 = y2;
        y2 = temp;
    }
    int dx = x2 - x1;
    int dy = y2 - y1;

    int dir = dx < 0 ? -1 : 1;
    dx *= dir;

    if (dy == 0) {
        my_mlx_pixel_put(&data, x1, y1, color);
        return;
    }
    int x = x1;
    int D = 2 * dx - dy;
    for (int i = 0; i <= dy; i++) {
        my_mlx_pixel_put(&data, x, y1 + i, color);
        if (D >= 0) {
            x += dir;
            D -= 2 * dy;
        }
        D += 2 * dx;
    }
}
// Uses Bresenham's to draw a line between 2 points in a grid, horzintal
void drawBetween2PointsHorz(int x1, int y1, int x2, int y2, unsigned int color, t_data data) {
    if (x1 > x2) {
        int temp = x1;
        x1 = x2;
        x2 = temp;
        temp = y1;
        y1 = y2;
        y2 = temp;
    }
    int dx = x2 - x1;
    int dy = y2 - y1;

    int dir = dy < 0 ? -1 : 1;
    dy *= dir;

    if (dx == 0) {
        my_mlx_pixel_put(&data, x1, y1, color);
        return;
    }
    int y = y1;
    int D = 2 * dy - dx;
    for (int i = 0; i <= dx; i++) {
        my_mlx_pixel_put(&data, x1 + i, y, color);
        if (D >= 0) {
            y += dir;
            D -= 2 * dx;
        }
        D += 2 * dy;
    }
}

// Uses Bresenham's to draw a line between 2 points in a grid, chooses H or V
void drawBetween2Points(int x1, int y1, int x2, int y2, unsigned int color, t_data data) {
    if (abs(x2 - x1) > abs(y2 - y1)) {
        drawBetween2PointsHorz(x1, y1, x2, y2, color, data);
    }
    else drawBetween2PointsVertic(x1, y1, x2, y2, color, data);
}


void drawGridIso(int rowLen, int colLen, int** grid, unsigned int color, t_data data) {
    double rad30 = 0.52359877559;
    double sin30 = sin(rad30);
    double cos30 = cos(rad30);

    int scale = 30;
    int z_scale = 2;
    int offset_x = 960;
    int offset_y = 300;

    for (int y = 0; y < colLen; y++) {
        for (int x = 0; x < rowLen; x++) {
            int currentX = (x - y) * scale * cos30 + offset_x;
            int currentY = (x + y) * scale * sin30 - (grid[y][x] * z_scale) + offset_y;

            // right neigh
            if (x < rowLen - 1) {
                int rightX = ((x + 1) - y) * scale * cos30 + offset_x;
                int rightY = ((x + 1) + y) * scale * sin30 - (grid[y][x + 1] * z_scale) + offset_y;
                drawBetween2Points(currentX, currentY, rightX, rightY, color, data);
            }

            // left neigh
            if (y < colLen - 1) {
                int bottomX = (x - (y + 1)) * scale * cos30 + offset_x;
                int bottomY = (x + (y + 1)) * scale * sin30 - (grid[y + 1][x] * z_scale) + offset_y;
                drawBetween2Points(currentX, currentY, bottomX, bottomY, color, data);
            }
        }
    }
}

int drawMap(char* file) {
    // inits
    t_data data;
    t_vars vars;
    int rowLen = -1;
    int colLen = 0;
    char* line;
    unsigned int color = 0x00FF0000;

    vars.mlx = mlx_init();
    vars.mlx_win = mlx_new_window(vars.mlx, 1920, 1080, "GRID");
    data.img = mlx_new_image(vars.mlx, 1920, 1080);
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
    int** grid = malloc(sizeof(int*) * len);
    if (grid == NULL) {
        printf("Error allocations on grid\n");
        return -1;
    }
    while(line != NULL) {
        printf("inLine\n");
        grid[colLen] = parseCords(line, &rowLen);
        line = get_next_line(fd);
        colLen++;
    }
    close(fd); // close file
    for (int i = 0; i < colLen; i++) {
        for (int e = 0; e < rowLen; e++) {
            printf("%d-", grid[i][e]);
        }
        printf("endRow\n");
    }
    // x = (x-y) * cos(30 deg)
    // y = (x+y) * sin(30 deg) - z
    // now having it integer grid, apply the formula on the fly and draw
    drawGridIso(rowLen, colLen, grid, color, data);
    mlx_put_image_to_window(vars.mlx, vars.mlx_win, data.img, 0, 0);
    mlx_loop(vars.mlx);
    return 0;
}