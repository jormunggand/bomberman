#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef __linux__
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#elif __APPLE__
#include <SDL.h>
#include <SDL_image.h>
#endif

// might be changed in the future
#define N_TEXTURES (10)
// the size of the board is currently fixed by defines but this will maybe be changed in the future to have variable sizes
#define TILE_SIZE (64)

typedef enum { // Tile types, will add the other types later
    HARD_WALL = 0,
    EMPTY = 1,
    SOFT_WALL = 2,
    PLAYER_POS = 3,
    BOMB_BONUS_HIDDEN = 4,
    BOMB_BONUS = 5,
    FLAME_BONUS_HIDDEN = 6,
    FLAME_BONUS = 7,
    SPEED_BONUS_HIDDEN = 8,
    SPEED_BONUS = 9
} TileType;

int min(int a, int b);
int max(int a, int b);
int init(SDL_Window **window, SDL_Renderer **renderer, int w, int h);
SDL_Texture* loadImage(const char path[], SDL_Renderer *renderer);
int setColor(SDL_Renderer *renderer, SDL_Color color);
