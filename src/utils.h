#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#ifdef __linux__
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_ttf.h>
#elif __APPLE__
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_net.h>
#include <SDL_ttf.h>
#endif

// might be changed in the future
#define N_TEXTURES (10)
// the size of the board is currently fixed by defines but this will maybe be changed in the future to have variable sizes
#define TILE_SIZE (48)

#define HUD_HEIGHT (50)

// Powerups constants
#define BASE_SPEED (128) // in pixels / second
#define SPEED_BOOST (32) // in pixels
#define MAX_SPEED (256)

#define MAX_BOMBS_PER_PLAYER (5)
#define MAX_POWER (5) // in number of tiles

typedef struct {
    int x;
    int y;
} Delta;

bool is_in_tab(int x, int* tab, int size);
int min(int a, int b);
int max(int a, int b);
int init(SDL_Window **window, SDL_Renderer **renderer, int w, int h);
SDL_Texture* loadImage(const char path[], SDL_Renderer *renderer);
int setColor(SDL_Renderer *renderer, SDL_Color color);
bool point_in_rect(SDL_Rect rect, int x, int y); // check wheter (x,y) is inside the rect
void gaussian_blur(Uint32 *pixels, int width, int height);
int renderTexture(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect *srcrect, SDL_Rect *dstrect);
