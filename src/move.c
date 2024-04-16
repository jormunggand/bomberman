// file meant to contain all functions related to moving the player
#include "move.h"
#include "utils.h"

float eps = 0.5 + TILE_SIZE / 4; // tolerance for collision detection

// moves the player with the veolicitites velx and vely if there is no collision that forbids it
// the collisions are calculated with collision_rect
void edge_collision(SDL_Window* window, SDL_Rect* player_rect, SDL_Rect* collision_rect, int** map, int velx, int vely) {
    int width, height;
    SDL_GetWindowSize(window, &width, &height);
    if (velx != 0){
        player_rect->x += velx;
        collision_rect->x += velx;
        if (collision_rect->x < 0 || collision_rect->x + collision_rect->w > width || check_collision(collision_rect, map)){
            player_rect->x -= velx;
            collision_rect->x -= velx;
        }
    }
    if (vely != 0){
        player_rect->y += vely;
        collision_rect->y += vely;
        if (collision_rect->y < 0 || collision_rect->y + collision_rect->h > height || check_collision(collision_rect, map)){
            collision_rect->y -= vely;
            player_rect->y -= vely;
        }
    }
}

// returns true if there is a collision between the rect and a wall
bool check_collision(SDL_Rect* r, int** map){
    for (int i = 0; i < 2; i++){
        for (int j = 0; j < 2; j++){
            int x = (r->x + i * (r->w - eps) + (1 - i) * eps) / TILE_SIZE;
            int y = (r->y + j * (r->h - eps)  + (1 - j) * eps) / TILE_SIZE;
            if (map[y][x] == HARD_WALL || map[y][x] == SOFT_WALL){
                return true;
            }
        }
    }
    return false;
}
