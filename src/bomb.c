#include "utils.h"
#include "bomb.h"

bool loadedTextures = false;
int tick_cycle = 1000;

SDL_Texture* bombTextures[NB_BOMB_TEXTURES];
SDL_Texture* flameTextures[NB_FLAME_TEXTURES];

// initialize a bomb at the given map coordinates and return it
void init_bomb(Bomb* bomb, int x, int y) {
    bomb->rect.x = x * TILE_SIZE;
    bomb->rect.y = y * TILE_SIZE;
    bomb->rect.w = TILE_SIZE;
    bomb->rect.h = TILE_SIZE;
    bomb->nb_ticks = 0;
    bomb->radius = 3;
}

// add a bomb to the map at the given map coordinates
void add_bomb(Map* map, int x, int y) {
    map->grid[y][x].bomb = malloc(sizeof(Bomb));
    init_bomb(map->grid[y][x].bomb, x, y);
}

// display a single bomb on the screen
// return 1 if the bomb animation is done and the bomb should be removed
int display_bomb(SDL_Renderer* render, Bomb* bomb) {
    if (!loadedTextures) {
        load_textures(render);
        loadedTextures = true;
    }
    if (bomb->nb_ticks < NB_BOMB_TEXTURES * tick_cycle){
        SDL_RenderCopy(render, bombTextures[bomb->nb_ticks / tick_cycle], NULL, &bomb->rect);
        bomb->nb_ticks++;
        return 0;
    }
    else if (bomb->nb_ticks < (NB_BOMB_TEXTURES + NB_FLAME_TEXTURES) * tick_cycle){
        SDL_RenderCopy(render, flameTextures[(bomb->nb_ticks - NB_BOMB_TEXTURES * tick_cycle) / tick_cycle], NULL, &bomb->rect);
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
                int r = display_bomb(render, map->grid[i][j].bomb);
                if (r == 1) {
                    free(map->grid[i][j].bomb);
                    map->grid[i][j].bomb = NULL;
                }
            }
        }
    }
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