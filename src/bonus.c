#include "bonus.h"

#include "player.h"
#include "map.h"

void get_bonus(Player* player, Map* map) {
    SDL_Rect* r = &player->collisionRect;
    for (int i = 0; i < 2; i++){
        for (int j = 0; j < 2; j++){
            int x = (r->x + i * (r->w - eps) + (1 - i) * eps) / TILE_SIZE;
            int y = (r->y + j * (r->h - eps)  + (1 - j) * eps) / TILE_SIZE;
            switch (map->grid[y][x].type) {
                case BOMB_BONUS:
                    player->nBombs = min(MAX_BOMBS, player->nBombs+1);
                    map->grid[y][x].type = EMPTY;
                    break;
                case FLAME_BONUS:
                    player->flamePower = min(MAX_POWER, player->flamePower+1);
                    map->grid[y][x].type = EMPTY;
                    break;
                case SPEED_BONUS:
                    player->speed = min(MAX_SPEED, player->speed + SPEED_BOOST);
                    map->grid[y][x].type = EMPTY;
                    break;
            }
        }
    }
}