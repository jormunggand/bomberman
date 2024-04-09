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

SDL_Texture* load_image(char* name, SDL_Renderer** render);
int setWindowColor(SDL_Renderer *renderer, SDL_Color color);
int init(SDL_Window **window, SDL_Renderer **renderer, int w, int h);