#include "util.c"

SDL_Texture load_image(char* name, ) {
    SDL_Surface* tmp = IMG_Load(name);
    if (tmp == NULL)
        printf("Error while loading file: %s", IMG_GetError)
}