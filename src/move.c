// file meant to contain all functions related to moving the player
#include "move.h"
#include "utils.h"


void edge_collision(SDL_Window* window, SDL_Rect* rect, int** map) {
    int width, height;
    SDL_GetWindowSize(window, &width, &height);
    int i = rect->x / TILE_SIZE;
    int j = rect->y / TILE_SIZE;
    int x_up = TILE_SIZE * i;
    int x_down = TILE_SIZE * (i + 1);
    int y_left = TILE_SIZE * j;
    int y_right = TILE_SIZE * (j + 1);
    if (i >= 1 && (map[i - 1][j] == HARD_WALL || map[i - 1][j] == SOFT_WALL) && rect->x < x_up){
        rect->x = x_up;
    }
    else if (i == 0 && rect->x < 0){
        rect->x = 0;
    }
    if (j >= 1 && (map[i][j - 1] == HARD_WALL || map[i][j - 1] == SOFT_WALL) && rect->y < y_left){
        rect->y = y_left;
    }
    else if (j == 0 && rect->y < 0){
        rect->y = 0;
    }
    if (i < MAP_SIZE - 1 && (map[i + 1][j] == HARD_WALL || map[i + 1][j] == SOFT_WALL) && rect->x + rect->w > x_down){
        rect->x = x_down - rect->w;
    }
    else if (i == MAP_SIZE - 1 && rect->x + rect->w > width){
        rect->x = width - rect->w;
    }
    if (j < MAP_SIZE - 1 && (map[i][j + 1] == HARD_WALL || map[i][j + 1] == SOFT_WALL) && rect->y + rect->h > y_right){
        rect->y = y_right - rect->h;
    }
    else if (j == MAP_SIZE - 1 && rect->y + rect->h > height){
        rect->y = height - rect->h;
    }
}
