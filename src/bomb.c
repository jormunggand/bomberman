#include "bomb.h"

#include "utils.h"
#include "player.h"
#include "map.h"

#include <assert.h>

#define BILLION (1000000000L)


const float bomb_hitbox_tolerance = TILE_SIZE / 16;
const double initial_bomb_time = 0.7;
const double bomb_cycle = 0.4;
const double flame_cycle = 0.3;


SDL_Texture* bombTextures[NB_BOMB_TEXTURES];
SDL_Texture* flameTextures[NB_FLAME_TEXTURES];

Bomb* bombs[MAX_BOMBS] = {NULL};

// initialize a bomb at the given map coordinates with a firepower of radius
// the bomb needs a pointer to the player who placed it to make sure he cannot
// place too many bombs
void init_bomb(Bomb* bomb, int x, int y, Player* owner) {
    bomb->rect.x = x * TILE_SIZE;
    bomb->rect.y = y * TILE_SIZE;
    bomb->rect.w = TILE_SIZE;
    bomb->rect.h = TILE_SIZE;

    bomb->collision_rect.x = x * TILE_SIZE + bomb_hitbox_tolerance;
    bomb->collision_rect.y = y * TILE_SIZE + bomb_hitbox_tolerance;
    bomb->collision_rect.w = TILE_SIZE - 2 * bomb_hitbox_tolerance;
    bomb->collision_rect.h = TILE_SIZE - 2 * bomb_hitbox_tolerance;

    clock_gettime(CLOCK_REALTIME, &(bomb->start_time));
    bomb->radius = owner->flamePower;
    bomb->detonated = false;
    bomb->explosion_tiles = calloc((4 * bomb->radius + 1), sizeof(bool));
    bomb->owner = owner;
    bomb->isMoving = false;
    bomb->direction = -1;
}

void player_place_bomb(Player* player, Map* map) {
    int x = (player->collisionRect.x + player->collisionRect.w / 2);
    int y = (player->collisionRect.y + player->collisionRect.h / 2);
    if (player->nCurBombs < player->nMaxBombs) {
        for (int i = 0; i < MAX_BOMBS; i++) {
            if (bombs[i] != NULL && point_in_rect(bombs[i]->rect, x, y)) {
                return; // the player is already standing on a bomb
            }
        }
        for (int i = 0; i < MAX_BOMBS; i++) {
            if (bombs[i] == NULL) { 
                bombs[i] = (Bomb*) malloc(sizeof(Bomb));
                assert(bombs[i] != NULL);
                init_bomb(bombs[i], x / TILE_SIZE, y / TILE_SIZE, player);
                player->nCurBombs++;
                break;
            }
        }
    }
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
    int ib = (bomb->rect.y + bomb->rect.h / 2) / TILE_SIZE; 
    int jb = (bomb->rect.x + bomb->rect.w / 2) / TILE_SIZE;
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
                            renderTexture(render, texture, NULL, &rect);
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
                            renderTexture(render, texture, NULL, &rect);
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
        bomb->isMoving = false;
    }
    else {
        int i, j;
        for (int k = 0; k < 4 * bomb->radius + 1; k++) {
            exlp_index_to_ij(k, bomb->radius, &i, &j);
            if (i + ib >= 0 && i + ib < map->size && j + jb >= 0 && j + jb < map->size){
                if (bomb->explosion_tiles[k]) {
                    SDL_Rect rect = {(j + jb) * TILE_SIZE, (i + ib) * TILE_SIZE, TILE_SIZE, TILE_SIZE};
                    renderTexture(render, texture, NULL, &rect);
                }
            }
        }
    }
}

void updateDeathStatus(Map* map, Player* player){
    for (int b = 0; b < MAX_BOMBS; b++){
        Bomb* bomb = bombs[b];
        if (bomb == NULL) continue;
        if (!bomb->detonated) continue;
        int ib = (bomb->rect.y + bomb->rect.h / 2) / TILE_SIZE; 
        int jb = (bomb->rect.x + bomb->rect.w / 2) / TILE_SIZE;
        int i, j;
        for (int k = 0; k < 4 * bomb->radius + 1; k++) {
            exlp_index_to_ij(k, bomb->radius, &i, &j);
            if (i + ib >= 0 && i + ib < map->size && j + jb >= 0 && j + jb < map->size){
                if (bomb->explosion_tiles[k]) {
                    SDL_Rect rect = {(j + jb) * TILE_SIZE, (i + ib) * TILE_SIZE, TILE_SIZE, TILE_SIZE};
                    for (int k = 0; k < 2; k++){
                        if (SDL_HasIntersection(&rect, &(player->flameHitbox)) == SDL_TRUE){
                            player->isAlive = false;
                            map->nPlayersAlive--;
                        }
                    }
                }
            }
        }
    }
}


// display a single bomb on the screen
// return 1 if the bomb animation is done and the bomb should be removed
int display_bomb(SDL_Renderer* render, Bomb* bomb, Map* map) {
    struct timespec cur_time;
    clock_gettime(CLOCK_REALTIME, &cur_time);
    double dt = (cur_time.tv_sec - bomb->start_time.tv_sec) + (double) (cur_time.tv_nsec - bomb->start_time.tv_nsec) / (double) BILLION;
    if (dt < initial_bomb_time) {
        renderTexture(render, bombTextures[0], NULL, &(bomb->rect));
        return 0;
    }
    else if (dt < initial_bomb_time + (NB_BOMB_TEXTURES - 1) * bomb_cycle){
        renderTexture(render, bombTextures[(int) ((dt - initial_bomb_time) / bomb_cycle)], NULL, &(bomb->rect));
        return 0;
    }
    else if (dt < NB_BOMB_TEXTURES * bomb_cycle + NB_FLAME_TEXTURES * flame_cycle){
        display_explosion(render, flameTextures[(int) ((dt - NB_BOMB_TEXTURES * bomb_cycle) / flame_cycle)], bomb, map);
        return 0;
    }
    else {
        return 1;
    }
    
}

// update the bomb positions according to their direction
void update_bombs_positions(SDL_Window* window, Map* map, double dt){
    int speed = 2 * MAX_SPEED;
    int width, height;
    SDL_GetWindowSize(window, &width, &height);
    int sign = 1;
    for (int k = 0; k < MAX_BOMBS; k++){
        Bomb* bomb = bombs[k];
        if (bomb == NULL) {continue;}
        if (bombs[k]->isMoving) {
            switch (bomb->direction){
                case FRONT:
                    bomb->rect.y += speed * dt;
                    bomb->collision_rect.y += speed * dt;
                    sign = 1;
                    break;
                case BACK:
                    bomb->rect.y -= speed * dt;
                    bomb->collision_rect.y -= speed * dt;
                    sign = -1;
                    break;
                case LEFT:
                    bomb->rect.x -= speed * dt;
                    bomb->collision_rect.x -= speed * dt;
                    sign = -1;
                    break;
                case RIGHT:
                    bomb->rect.x += speed * dt;
                    bomb->collision_rect.x += speed * dt;
                    sign = 1;
                    break;
            }
            if (bomb->rect.x < 0 
            || bomb->rect.x + bomb->rect.w > width 
            || check_collision(&(bomb->collision_rect), map)
            || bombToBombCollision(&(bomb->collision_rect), sign, k)){
                bombs[k]->isMoving = false;
                switch(bombs[k]->direction){
                    case FRONT:
                        bomb->rect.y -= speed * dt;
                        bomb->collision_rect.y -= speed * dt;
                        break;
                    case BACK:
                        bomb->rect.y += speed * dt;
                        bomb->collision_rect.y += speed * dt;
                        break;
                    case LEFT:
                        bomb->rect.x += speed * dt;
                        bomb->collision_rect.x += speed * dt;
                        break;
                    case RIGHT:
                        bomb->rect.x -= speed * dt;
                        bomb->collision_rect.x -= speed * dt;
                        break;
                }
            }
        }
    }
}

// display all bombs present on the map
void display_bombs(SDL_Renderer* render, Map* map) {
    for (int k = 0; k < MAX_BOMBS; k++) {
        if (bombs[k] == NULL) {continue;}
        int r = display_bomb(render, bombs[k], map);
        if (r == 1) {
            bombs[k]->owner->nCurBombs--;
            free(bombs[k]->explosion_tiles);
            free(bombs[k]);
            bombs[k] = NULL;
        }
    }
}

int load_bomb_aux(SDL_Renderer* render, char* base, SDL_Texture** textures, int nb_textures) {
    for (int i = 0; i < nb_textures; i++) {
        char filename[50];
        sprintf(filename, base, i);
        SDL_Texture* curText = loadImage(filename, render, false);
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

void free_bombs(){
    for (int i = 0; i < MAX_BOMBS; i++){
        if (bombs[i] != NULL){
            free(bombs[i]->explosion_tiles);
            free(bombs[i]);
            bombs[i] = NULL;
        }
    }
}


// to handle collision between bombs
bool bombToBombCollision(SDL_Rect* r, int sign, int bombIndex){
    SDL_Rect intersectRect;
    for (int k = 0; k < MAX_BOMBS; k++){
        if (k == bombIndex || bombs[k] == NULL) continue;
        if (!(bombs[k]->detonated) 
        && SDL_IntersectRect(r, &(bombs[k]->rect), &intersectRect) == SDL_TRUE){
            if (bombs[k]->isMoving){
                bombs[k]->isMoving = false;
            }
            else{
                SpriteDirection d;
                if (sign == 1){
                    d = intersectRect.w > intersectRect.h ? FRONT : RIGHT;
                }
                else{
                    d = intersectRect.w > intersectRect.h ? BACK : LEFT;
                }
                bombs[k]->isMoving = true;
                bombs[k]->direction = d;
            }
            return true;
        }
    }
    return false;
}