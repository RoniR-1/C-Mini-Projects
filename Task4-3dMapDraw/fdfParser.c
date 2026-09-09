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
    printf("%d\n", len);
    *size = len;
    int* result = malloc(sizeof(int) * len);

    len = 0;
    counter = 0;
    while(s[counter]) {
        if (s[counter] != ' ' && s[counter] != '\n') {
            int x = 0;
            while(s[counter] && s[counter] != ' ' && s[counter] != '\n') {
                x = x * 10 + s[counter] - '0';
                counter++;
            }
            result[len] = x;
            len++;
        }
        else counter++;
    }
    return result;
}

void my_mlx_pixel_put(t_data *data, int x, int y, int color) {
    printf("%d-%d\n", x, y);
	char* dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

void drawBetween2Points(int x1, int y1, int x2, int y2, int color, t_data data) {
    if (x1 > x2) {
        int temp = x1;
        x1 = x2;
        x2 = temp;
        temp = y1;
        y1 = y2;
        y2 = temp;
    }

    
    int dx = x1 - x2;
    int dy = y1 - y2;

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



void drawGridIso(int rowLen, int colLen, int** grid, int color, t_data data) {
    // x = (x-y) * cos(30 deg)
    // y = (x+y) * sin(30 deg) - z
    // Need to draw line from iso current to iso current right neigh and under neigh
    double sin30 = sin(30);
    double cos30 = cos(30);
    for (int y = 0; y < colLen; y++) {
        for (int x = 0; x < rowLen; x++) {
            int currentX = (x-y) * cos30;
            int currentY = (x+y) * sin30 - grid[y][x];
            if (x < rowLen - 1) {
                drawBetween2Points(currentX, currentY, (x+1-y) * cos30, (x+1+y) * sin30 - grid[y][x+1], color, data);
            }
            if (y < colLen - 1) {
                drawBetween2Points(currentX, currentY, (x-y+1) * cos30, (x+y+1) * sin30 - grid[y+1][x], color, data);
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
    int color = 0x00FF0000;

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
    int** grid = malloc(sizeof(int*) * 10);
    if (grid == NULL) {
        printf("Error allocations on grid\n");
        return -1;
    }
    line = get_next_line(fd);
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