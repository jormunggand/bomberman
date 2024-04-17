// file meant to contain all functions related to moving the player
#include "move.h"
#include "utils.h"

bool loadedAnim = false;

SDL_Texture* front_walking[ANIMATION_FRAMES];
SDL_Texture* back_walking[ANIMATION_FRAMES];
SDL_Texture* side_walking[ANIMATION_FRAMES];


void init_player(Player* player, int x, int y) {
    player->curDir = FRONT;
    player->iframe = 0;
    player->isWalking = false;

    player->rect.x = x;
    player->rect.y = y;
    player->rect.w = TILE_SIZE;
    player->rect.h = 2 * TILE_SIZE;

    player->animations = front_walking;
}

void display_player(SDL_Renderer* render, Player* player) {
    if (!loadedAnim) {
        loadedAnim = true;
        load_animations(render);
    }
    if (!player->isWalking) player->iframe = 0;
    if (player->curDir == LEFT) {
        SDL_RenderCopyEx(render, player->animations[player->iframe], NULL, &(player->rect), 
            0, NULL, SDL_FLIP_HORIZONTAL);
    } else {
        SDL_RenderCopy(render, player->animations[player->iframe], NULL, &(player->rect));
    }
}

void change_direction(Player* player, SpriteDirection newDir) {
    player->curDir = newDir;
    if (player->curDir == FRONT) {
        player->animations = front_walking;
    } else if (player->curDir == BACK) {
        player->animations = back_walking;
    } else {
        player->animations = side_walking;
    }
}


void update_sprite(Player* player) {
    player->iframe++;
    player->iframe %= ANIMATION_FRAMES;
}

// void destroy_player(Player* player) {
//     free(player->rect);
//     free(player);
// }


int load_animations_aux(SDL_Renderer* render, char* base, SDL_Texture** textures) {
    for (int i = 0; i < 8; i++) {
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

int load_animations(SDL_Renderer* render) {
    int r1, r2, r3;
    r1 = load_animations_aux(render, "../assets/Bomberman/Front/Bman_F_f0%d.png", front_walking);
    r2 = load_animations_aux(render, "../assets/Bomberman/Back/Bman_B_f0%d.png", back_walking);
    r3 = load_animations_aux(render, "../assets/Bomberman/Side/Bman_F_f0%d.png", side_walking);
    if (r1 + r2 + r3 != 0) {
        return -1;
    }
    return 0;
}

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
        collision_rect->y += vely;
        player_rect->y += vely;
        if (collision_rect->y < 0 || collision_rect->y + collision_rect->h > height || check_collision(collision_rect, map)){
            collision_rect->y -= vely;
            player_rect->y -= vely;
        }
    }
}

bool check_collision(SDL_Rect* r, int** map) {
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
