#include "utils.h"
#include "bomb.h"

bool loadedTextures = false;
int tick_cycle = 600;

SDL_Texture* bombTextures[NB_BOMB_TEXTURES];
SDL_Texture* flameTextures[NB_FLAME_TEXTURES];

void init_bomb(Bomb* bomb, int x, int y) {
    bomb->rect.x = x;
    bomb->rect.y = y;
    bomb->rect.w = TILE_SIZE;
    bomb->rect.h = TILE_SIZE;
    bomb->nb_ticks = 0;
    bomb->radius = 3;
}

void display_bomb(SDL_Renderer* render, Bomb* bomb) {
    if (!loadedTextures) {
        load_textures(render);
        loadedTextures = true;
    }
    if (bomb->nb_ticks < NB_BOMB_TEXTURES * tick_cycle){
        SDL_RenderCopy(render, bombTextures[bomb->nb_ticks / tick_cycle], NULL, &bomb->rect);
    }
    else{
        SDL_RenderCopy(render, flameTextures[(bomb->nb_ticks - NB_BOMB_TEXTURES * tick_cycle) / tick_cycle], NULL, &bomb->rect);
    }
    bomb->nb_ticks++;
}

int load_textures_aux(SDL_Renderer* render, char* base, SDL_Texture** textures, int nb_textures) {
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

int load_textures(SDL_Renderer* render) {
    int r1, r2;
    r1 = load_textures_aux(render, "../assets/Bomb/Bomb_f0%d.png", bombTextures, NB_BOMB_TEXTURES);
    r2 = load_textures_aux(render, "../assets/Flame/Flame_f0%d.png", flameTextures, NB_FLAME_TEXTURES);
    if (r1 + r2 != 0) {
        return -1;
    }
    return 0;
}