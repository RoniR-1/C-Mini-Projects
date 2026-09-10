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
void drawBetween2PointsVertic(int x1, int y1, int x2, int y2, unsigned int color, t_data* data) {
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
        my_mlx_pixel_put(data, x1, y1, color);
        return;
    }
    int x = x1;
    int D = 2 * dx - dy;
    for (int i = 0; i <= dy; i++) {
        my_mlx_pixel_put(data, x, y1 + i, color);
        if (D >= 0) {
            x += dir;
            D -= 2 * dy;
        }
        D += 2 * dx;
    }
}
// Uses Bresenham's to draw a line between 2 points in a grid, horzintal
void drawBetween2PointsHorz(int x1, int y1, int x2, int y2, unsigned int color, t_data* data) {
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
        my_mlx_pixel_put(data, x1, y1, color);
        return;
    }
    int y = y1;
    int D = 2 * dy - dx;
    for (int i = 0; i <= dx; i++) {
        my_mlx_pixel_put(data, x1 + i, y, color);
        if (D >= 0) {
            y += dir;
            D -= 2 * dx;
        }
        D += 2 * dy;
    }
}

// Uses Bresenham's to draw a line between 2 points in a grid, chooses H or V
void drawBetween2Points(int x1, int y1, int x2, int y2, unsigned int color, t_data* data) {
    if (abs(x2 - x1) > abs(y2 - y1)) {
        drawBetween2PointsHorz(x1, y1, x2, y2, color, data);
    }
    else drawBetween2PointsVertic(x1, y1, x2, y2, color, data);
}

void processPoint(int rowLen, int colLen, int x, int y, int z, double rotate_rad_X, double rotate_rad_Y, double rotate_rad_Z, int* resultX, int* resultY) {
    double shift_x = x - ((rowLen-1) / 2.0);
    double shift_y = y - ((colLen-1) / 2.0);
    double shift_z = z*2;

    // vertical rotation, X axis
    double vert_y = shift_y * cos(rotate_rad_X) - shift_z * sin(rotate_rad_X);
    double vert_z = shift_y * sin(rotate_rad_X) + shift_z * cos(rotate_rad_X);

    //roll rotation, Y axis
    double roll_x = shift_x * cos(rotate_rad_Y) + vert_z * sin(rotate_rad_Y);
    double roll_z = -shift_x * sin(rotate_rad_Y) + vert_z * cos(rotate_rad_Y);

    //Horizantal rotation, Z axis
    double rotate_x = roll_x * cos(rotate_rad_Z) - vert_y * sin(rotate_rad_Z);
    double rotate_y = roll_x * sin(rotate_rad_Z) + vert_y * cos(rotate_rad_Z);

    double max_dim = (rowLen > colLen) ? rowLen : colLen;
    double compress = rotate_rad_X > 0 || rotate_rad_Y > 0 ? 100 : 0;
    double scale = 800.0 / (max_dim * 1.732 + compress);
    int offset_x = 960;
    int offset_y = 500;
    double rad30 = 0.52359877559;

    *resultX = (int)round((rotate_x - rotate_y) * scale * cos(rad30)) + offset_x;
    *resultY = (int)round((rotate_x + rotate_y) * scale * sin(rad30) - roll_z) + offset_y;
}


void drawGridIso(t_grid* grid_data, t_data* data) {
    double rad1 = 0.01745329252;
    double rotate_X = grid_data->angleX * rad1;
    double rotate_Y = grid_data->angleY * rad1;
    double rotate_Z = grid_data->angleZ * rad1;
    

    for (int y = 0; y < grid_data->colLen; y++) {
        for (int x = 0; x < grid_data->rowLen; x++) {
            int cx, cy;
            processPoint(grid_data->rowLen, grid_data->colLen, x, y, grid_data->grid[y][x], rotate_X, rotate_Y, rotate_Z, &cx, &cy);
            // right neigh
            if (x < grid_data->rowLen - 1) {
                int neigh_x = x + 1;
                int neigh_y = y + 1; // fallback in case not initlized 
                processPoint(grid_data->rowLen, grid_data->colLen, x+1, y, grid_data->grid[y][x+1], rotate_X, rotate_Y, rotate_Z, &neigh_x, &neigh_y);
                drawBetween2Points(cx, cy, neigh_x, neigh_y, grid_data->color, data);
            }

            // under neigh
            if (y < grid_data->colLen - 1) {
                int neigh_x = x + 1;
                int neigh_y = y + 1; // fallback in case not initlized                
                processPoint(grid_data->rowLen, grid_data->colLen, x, y+1, grid_data->grid[y+1][x], rotate_X, rotate_Y, rotate_Z, &neigh_x, &neigh_y);
                drawBetween2Points(cx, cy, neigh_x, neigh_y, grid_data->color, data);
            }
        }
    }
}
