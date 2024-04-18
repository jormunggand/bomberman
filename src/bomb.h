#ifdef __linux__
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#elif __APPLE__
#include <SDL.h>
#include <SDL_image.h>
#endif

#define NB_BOMB_TEXTURES (2)
#define NB_FLAME_TEXTURES (5)

#ifndef MAP_H
#define MAP_H
#include "map.h"
#endif

#ifndef MOVE_H
#define MOVE_H
#include "move.h"
#endif

int ij_to_expl_index(int i, int j, int radius);
void exlp_index_to_ij(int expl_index, int radius, int* i, int* j);

void init_bomb(Bomb* bomb, int x, int y);
void add_bomb(Map* map, int x, int y);
void player_place_bomb(Player* player, Map* map);
void display_explosion(SDL_Renderer* render, SDL_Texture* texture, Bomb* bomb, Map* map);
int display_bomb(SDL_Renderer* render, Bomb* bomb, Map* map);
void display_bombs(SDL_Renderer* render, Map* map);
int load_textures_aux(SDL_Renderer* render, char* base, SDL_Texture** textures, int nb_textures);
int load_textures(SDL_Renderer* render);