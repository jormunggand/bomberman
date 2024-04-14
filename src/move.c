// file meant to contain all functions related to moving the player
#include "move.h"
#include "utils.h"

bool loadedAnim = false;

SDL_Texture* front_walking[ANIMATION_FRAMES];
SDL_Texture* back_walking[ANIMATION_FRAMES];
SDL_Texture* side_walking[ANIMATION_FRAMES];


Player* create_player(int x, int y) {
    Player* player = malloc(sizeof(player));
    player->curDir = FRONT;
    player->iframe = 0;

    player->rect = malloc(sizeof(SDL_Rect));
    player->rect->x = x;
    player->rect->y = y;
    player->rect->w = 64;
    player->rect->h = 128;

    player->animations = front_walking;

    return player;
}

void display_player(SDL_Renderer* render, Player* player) {
    if (!loadedAnim) {
        loadedAnim = true;
        load_animations(render);
    }
    SDL_RenderCopy(render, player->animations[player->iframe], NULL, player->rect);
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
    player->iframe = 0;
}


void update_sprite(SDL_Renderer* render, Player* player) {
    player->iframe  = (player->iframe + 1) % ANIMATION_FRAMES;
    if (player->curDir == LEFT) {

    } else {
         
    }
}

void destroy_player(Player* player) {
    free(player->rect);
    free(player);
}


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
