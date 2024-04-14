// file meant to contain all functions related to moving the player
#include "move.h"
#include "utils.h"


void edge_collision(SDL_Window* window, SDL_Rect* rect, int** map, int velx, int vely) {
    int width, height;
    SDL_GetWindowSize(window, &width, &height);
    if (velx != 0){
        rect->x += velx;
        if (rect->x < 0 || rect->x + rect->h > height || check_collision(rect, map)){
            rect->x -= velx;
        }
    }
    if (vely != 0){
        rect->y += vely;
        if (rect->y < 0 || rect->y + rect->w > width || check_collision(rect, map)){
            rect->y -= vely;
        }
    }
}

bool check_collision(SDL_Rect* r, int** map){
    for (int i = 0; i < 2; i++){
        for (int j = 0; j < 2; j++){
            int x = (r->x + i * r->w) / TILE_SIZE;
            int y = (r->y + j * r->h) / TILE_SIZE;
            if (map[x][y] == HARD_WALL || map[x][y] == SOFT_WALL){
                return true;
            }
        }
    }
    return false;
}
