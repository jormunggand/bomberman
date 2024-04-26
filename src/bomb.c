#include "bomb.h"

#include "utils.h"
#include "player.h"
#include "map.h"


int bomb_cycle = 2600;
int flame_cycle = 500;

SDL_Texture* bombTextures[NB_BOMB_TEXTURES];
SDL_Texture* flameTextures[NB_FLAME_TEXTURES];

// initialize a bomb at the given map coordinates and return it
void init_bomb(Bomb* bomb, int x, int y, int radius) {
    bomb->rect.x = x * TILE_SIZE;
    bomb->rect.y = y * TILE_SIZE;
    bomb->rect.w = TILE_SIZE;
    bomb->rect.h = TILE_SIZE;
    bomb->nb_ticks = 0;
    bomb->radius = radius;
    bomb->detonated = false;
    bomb->explosion_tiles = calloc((4 * bomb->radius + 1), sizeof(bool));
}

void player_place_bomb(Player* player, Map* map) {
    int x = (player->collisionRect.x + player->collisionRect.w / 2) / TILE_SIZE;
    int y = (player->collisionRect.y + player->collisionRect.h / 2) / TILE_SIZE;
    if (map->grid[y][x].bomb == NULL) {
        add_bomb(map, x, y, player->flamePower);
    }
}

// add a bomb to the map at the given map coordinates
void add_bomb(Map* map, int x, int y, int radius) {
    map->grid[y][x].bomb = malloc(sizeof(Bomb));
    init_bomb(map->grid[y][x].bomb, x, y, radius);
}

int ij_to_expl_index(int i, int j, int radius) {
    if (j == 0) return i + radius;
    else if (j < 0) return 2 * radius - j;
    else return 3 * radius + j;
}

void exlp_index_to_ij(int expl_index, int radius, int* i, int* j) {
    if (expl_index < 2 * radius + 1) {
        *i = expl_index - radius;
        *j = 0;
    }
    else if (expl_index < 3 * radius + 1) {
        *i = 0;
        *j = 2 * radius - expl_index;
    }
    else {
        *i = 0;
        *j = expl_index - 3 * radius;
    }
}

void display_explosion(SDL_Renderer* render, SDL_Texture* texture, Bomb* bomb, Map* map){
    int r = bomb->radius;
    int ib = bomb->rect.y / TILE_SIZE; 
    int jb = bomb->rect.x / TILE_SIZE;
    bool flag = false;
    if (!bomb->detonated) {
        for (int sgn = -1; sgn <= 1; sgn += 2) {
            for (int k = 0; k <= r; k++) {
                int i = sgn * k;
                if (i == 0 && sgn == 1) continue; // avoid drawing the center twice
                if (i + ib >= 0 && i + ib < map->size){
                    SDL_Rect rect = {jb * TILE_SIZE, (i + ib) * TILE_SIZE, TILE_SIZE, TILE_SIZE};
                    switch (map->grid[i + ib][jb].type){
                        case HARD_WALL:
                            flag = true; // to stop the loop
                            break;
                        case SOFT_WALL:
                            map->grid[i+ib][jb].type = EMPTY;
                            flag = true; // to stop the loop
                        case EMPTY:
                        case PLAYER_SPAWN:
                            bomb->explosion_tiles[ij_to_expl_index(i, 0, bomb->radius)] = true;
                            if (map->grid[i + ib][jb].bonus != NONE && !flag) {map->grid[i + ib][jb].bonus = NONE;}
                            SDL_RenderCopy(render, texture, NULL, &rect);
                            break;
                        default:
                            break;
                    }
                    if (flag) {
                        flag = false;
                        break;
                    }
                }
            }
            for (int k = 1; k <= r; k++) {
                int j = sgn * k;
                if (j + jb >= 0 && j + jb < map->size) {
                    SDL_Rect rect = {(j + jb) * TILE_SIZE, ib * TILE_SIZE, TILE_SIZE, TILE_SIZE};
                    switch (map->grid[ib][j + jb].type){
                        case HARD_WALL:
                            flag = true; // to stop the loop
                            break;
                        case SOFT_WALL:
                            map->grid[ib][j + jb].type = EMPTY;
                            flag = true; // to stop the loop
                        case EMPTY:
                        case PLAYER_SPAWN:
                            bomb->explosion_tiles[ij_to_expl_index(0, j, bomb->radius)] = true;
                            if (map->grid[ib][j + jb].bonus != NONE && !flag) {map->grid[ib][j + jb].bonus = NONE;}
                            SDL_RenderCopy(render, texture, NULL, &rect);
                            break;
                        default:
                            break;
                    }
                    if (flag) {
                        flag = false;
                        break;
                    }
                }
            }
        }
        bomb->detonated = true;
    }
    else {
        int i, j;
        for (int k = 0; k < 4 * bomb->radius + 1; k++) {
            exlp_index_to_ij(k, bomb->radius, &i, &j);
            if (i + ib >= 0 && i + ib < map->size && j + jb >= 0 && j + jb < map->size){
                if (bomb->explosion_tiles[k]) {
                    SDL_Rect rect = {(j + jb) * TILE_SIZE, (i + ib) * TILE_SIZE, TILE_SIZE, TILE_SIZE};
                    SDL_RenderCopy(render, texture, NULL, &rect);
                }
            }
        }
    }
}

// display a single bomb on the screen
// return 1 if the bomb animation is done and the bomb should be removed
int display_bomb(SDL_Renderer* render, Tile* tile, Map* map) {
    Bomb* bomb = tile->bomb;
    if (bomb->nb_ticks < NB_BOMB_TEXTURES * bomb_cycle){
        SDL_RenderCopy(render, bombTextures[bomb->nb_ticks / bomb_cycle], NULL, &bomb->rect);
        bomb->nb_ticks++;
        return 0;
    }
    else if (bomb->nb_ticks < NB_BOMB_TEXTURES * bomb_cycle + NB_FLAME_TEXTURES * flame_cycle){
        display_explosion(render, flameTextures[(bomb->nb_ticks - NB_BOMB_TEXTURES * bomb_cycle) / flame_cycle], bomb, map);
        bomb->nb_ticks++;   
        return 0;
    }
    else {
        return 1;
    }
}

// display all bombs present on the map
void display_bombs(SDL_Renderer* render, Map* map) {
    for (int i = 0; i < map->size; i++) {
        for (int j = 0; j < map->size; j++) {
            if (map->grid[i][j].bomb != NULL) {
                int r = display_bomb(render, &map->grid[i][j], map);
                if (r == 1) {
                    free(map->grid[i][j].bomb->explosion_tiles);
                    free(map->grid[i][j].bomb);
                    map->grid[i][j].bomb = NULL;
                }
            }
        }
    }
}

int load_bomb_aux(SDL_Renderer* render, char* base, SDL_Texture** textures, int nb_textures) {
    for (int i = 0; i < nb_textures; i++) {
        char filename[50];
        sprintf(filename, base, i);
        SDL_Texture* curText = loadImage(filename, render);
        if (curText == NULL) {
            printf("%s\n", SDL_GetError());
            return -1;
        }
        textures[i] = curText;
        }
    return 0;
}

int load_bomb_textures(SDL_Renderer* render) {
    int r1, r2;
    r1 = load_bomb_aux(render, "../assets/Bomb/Bomb_f0%d.png", bombTextures, NB_BOMB_TEXTURES);
    r2 = load_bomb_aux(render, "../assets/Flame/Flame_f0%d.png", flameTextures, NB_FLAME_TEXTURES);
    if (r1 + r2 != 0) {
        return -1;
    }
    return 0;
}