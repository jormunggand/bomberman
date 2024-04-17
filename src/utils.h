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
#define N_TEXTURES (5)
// the size of the board is currently fixed by defines but this will maybe be changed in the future to have variable sizes
#define TILE_SIZE (64)

typedef enum { // Tile types, will add the other types later
    HARD_WALL,
    EMPTY,
    SOFT_WALL
} TileType;

int init(SDL_Window **window, SDL_Renderer **renderer, int w, int h);
SDL_Texture* loadImage(const char path[], SDL_Renderer *renderer);
int setColor(SDL_Renderer *renderer, SDL_Color color);
