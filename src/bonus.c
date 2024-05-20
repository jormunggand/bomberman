#include "bonus.h"

#include "utils.h"
#include "player.h"
#include "map.h"

#include <time.h>
#include <stdlib.h>

#define SPAWN_RATE (4)


void init_bonus(Map* map) {
    srandom(time(NULL));

    for (int y = 0; y < map->size; y++) {
        for (int x = 0; x < map->size; x++) {
            if (map->grid[y][x].type == SOFT_WALL) {
                if (random() % 10 > SPAWN_RATE) {
                    switch (random() % 3) {
                        case (0):
                            map->grid[y][x].bonus = BOMB_BONUS;
                            break;
                        case (1):
                            map->grid[y][x].bonus = SPEED_BONUS;
                            break;
                        case (2):
                            map->grid[y][x].bonus = FLAME_BONUS;
                            break;
                    }
                }
            }
        }
    }
}


// check if the player is touching a bonus
// not at all inspired by check_collision's code
void get_bonus(Player* player, Map* map) {
    SDL_Rect* r = &player->flameHitbox;
    for (int i = 0; i < 2; i++){
        for (int j = 0; j < 2; j++){
            int x = (r->x + i * r->w ) / TILE_SIZE;
            int y = (r->y + j * r->h) / TILE_SIZE;
            switch (map->grid[y][x].bonus) {
                case BOMB_BONUS:
                    player->nMaxBombs = min(MAX_BOMBS_PER_PLAYER, player->nMaxBombs+1);
                    map->grid[y][x].bonus = NONE;
                    break;
                case FLAME_BONUS:
                    player->flamePower = min(MAX_POWER, player->flamePower+1);
                    map->grid[y][x].bonus = NONE;
                    break;
                case SPEED_BONUS:
                    player->speed = min(MAX_SPEED, player->speed + SPEED_BOOST);
                    map->grid[y][x].bonus = NONE;
                    break;
                default:
                    break;
            }
        }
    }
}