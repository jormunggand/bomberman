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

// checks is x is in the tab array 
bool is_in_tab(int x, int* tab, int size);

// returns the minimum of a and b
int min(int a, int b);

// returns the maximum of a and b
int max(int a, int b);

// initialize the SDL window and renderer
int init(SDL_Window **window, SDL_Renderer **renderer, int w, int h);

// load an image from the path and return a texture, if isNegative is true, the image's colors will be inverted
SDL_Texture* loadImage(const char path[], SDL_Renderer *renderer, bool isNegative);

int setColor(SDL_Renderer *renderer, SDL_Color color);

// check whether (x,y) is inside the rect
bool point_in_rect(SDL_Rect rect, int x, int y); 

// apply a gaussian blur on the pixels array, radius used is 10
void gaussian_blur(Uint32 *pixels, int width, int height);

// render a texture on the screen but take into account the offset of the hud
int renderTexture(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect *srcrect, SDL_Rect *dstrect);
