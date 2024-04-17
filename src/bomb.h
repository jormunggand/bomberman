#ifdef __linux__
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#elif __APPLE__
#include <SDL.h>
#include <SDL_image.h>
#endif

#define NB_BOMB_TEXTURES (2)
#define NB_FLAME_TEXTURES (5)

typedef struct Bomb {
    SDL_Rect rect;
    int nb_ticks;
    int radius;
} Bomb;

void init_bomb(Bomb* bomb, int x, int y);
void display_bomb(SDL_Renderer* render, Bomb* bomb);
int load_textures_aux(SDL_Renderer* render, char* base, SDL_Texture** textures, int nb_textures);
int load_textures(SDL_Renderer* render);