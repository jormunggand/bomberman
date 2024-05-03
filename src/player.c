// file meant to contain all functions related to moving the player
#include "player.h"

#include "utils.h"
#include "map.h"
#include "bomb.h"
#include "keyboard.h"


SDL_Texture* front_walking[ANIMATION_FRAMES];
SDL_Texture* back_walking[ANIMATION_FRAMES];
SDL_Texture* left_walking[ANIMATION_FRAMES];
SDL_Texture* right_walking[ANIMATION_FRAMES];


// create a player structure positioned at map.grid[y][x]
void init_player(Player* player, int x, int y, Key* controls) {
    player->curDir = FRONT;
    player->iframe = 0;
    player->isWalking = false;
    player->cpt_reset = 0;

    player->rect.w = TILE_SIZE;
    player->rect.h = 2 * TILE_SIZE;
    player->rect.x = x * TILE_SIZE;
    player->rect.y = (y - 1) * TILE_SIZE;
    

    player->collisionRect.x = x * TILE_SIZE;
    player->collisionRect.y = (y - 1) * TILE_SIZE + 5*player->rect.h/8;
    player->collisionRect.w = TILE_SIZE;
    player->collisionRect.h = TILE_SIZE;

    player->animations = front_walking;

    player->nMaxBombs = 1;
    player->nCurBombs = 0;
    player->speed = BASE_SPEED;
    player->flamePower = 1;

    for (int i = 0; i < 5; i++)
        player->controls[i] = controls[i];
}

void display_player(SDL_Renderer* render, Player* player) {
    if (!player->isWalking) player->iframe = 0;
    SDL_RenderCopy(render, player->animations[player->iframe], NULL, &(player->rect));
}

void change_direction(Player* player, SpriteDirection newDir) {
    player->curDir = newDir;
    if (player->curDir == FRONT) {
        player->animations = front_walking;
    } else if (player->curDir == BACK) {
        player->animations = back_walking;
    } else if (player->curDir == LEFT) {
        player->animations = left_walking;
    }
    else {
        player->animations = right_walking;
    }
}


void update_sprite(Player* player) {
    player->iframe++;
    player->iframe %= ANIMATION_FRAMES;
}

int load_player_aux(SDL_Renderer* render, char* base, SDL_Texture** textures) {
    for (int i = 0; i < ANIMATION_FRAMES; i++) {
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

int load_player_textures(SDL_Renderer* render) {
    int r1, r2, r3, r4;
    r1 = load_player_aux(render, "../assets/Bomberman/Front/Bman_F_f0%d.png", front_walking);
    r2 = load_player_aux(render, "../assets/Bomberman/Back/Bman_B_f0%d.png", back_walking);
    r3 = load_player_aux(render, "../assets/Bomberman/Left/Bman_L_f0%d.png", left_walking);
    r4 = load_player_aux(render, "../assets/Bomberman/Right/Bman_R_f0%d.png", right_walking);
    if (r1 + r2 + r3 + r4 != 0) {
        return -1;
    }
    return 0;
}

float eps = 0.5 + TILE_SIZE / 4; // tolerance for collision detection

// moves the player with the veolicitites velx and vely if there is no collision that forbids it
// the collisions are calculated with collision_rect
void edge_collision(SDL_Window* window, Player* player, Map *map, int deltaX, int deltaY, double deltaTime) {
    int width, height;
    SDL_GetWindowSize(window, &width, &height);
    SDL_Rect* player_rect = &player->rect;
    SDL_Rect* collision_rect = &player->collisionRect;
    if (deltaX != 0){
        int dx = (int) (deltaX * deltaTime * player->speed);
        bool collidedwithBomb = bomb_collision(collision_rect, map);
        player_rect->x += dx;
        collision_rect->x += dx;
        if (collision_rect->x < 0 || collision_rect->x + collision_rect->w > width 
            || check_collision(collision_rect, map)
            || (!collidedwithBomb && bomb_collision(collision_rect, map))){
            player_rect->x -= dx;
            collision_rect->x -= dx;
        }
    }
    if (deltaY != 0){
        int dy = (int) (deltaY * deltaTime * player->speed);
        bool collidedwithBomb = bomb_collision(collision_rect, map);
        collision_rect->y += dy;
        player_rect->y += dy;
        if (collision_rect->y < 0 || collision_rect->y + collision_rect->h > height 
            || check_collision(collision_rect, map)
            || (!collidedwithBomb && bomb_collision(collision_rect, map))){
            collision_rect->y -= dy;
            player_rect->y -= dy;
        }
    }
}

bool check_collision(SDL_Rect* r, Map *map) {
    for (int i = 0; i < 2; i++){
        for (int j = 0; j < 2; j++){
            int x = (r->x + i * (r->w - eps) + (1 - i) * eps) / TILE_SIZE;
            int y = (r->y + j * (r->h - eps)  + (1 - j) * eps) / TILE_SIZE;
            if (map->grid[y][x].type == HARD_WALL || map->grid[y][x].type == SOFT_WALL){
                return true;
            }
        }
    }
    return false;
}

// check if the rectangle is colliding with a bomb
bool bomb_collision(SDL_Rect* r, Map *map) {
    for (int i = 0; i < 2; i++){
        for (int j = 0; j < 2; j++){
            int x = (r->x + i * (r->w - eps) + (1 - i) * eps) / TILE_SIZE;
            int y = (r->y + j * (r->h - eps)  + (1 - j) * eps) / TILE_SIZE;
            if (map->grid[y][x].bomb != NULL && !(map->grid[y][x].bomb->detonated)){
                return true;
            }
        }
    }
    return false;
}

