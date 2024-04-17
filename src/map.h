#ifdef __linux__
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#elif __APPLE__
#include <SDL.h>
#include <SDL_image.h>
#endif

typedef struct Bomb {
    SDL_Rect rect;
    int nb_ticks;
    int radius;
} Bomb;

typedef struct Tile {
    int type;
    Bomb* bomb;
} Tile;

typedef struct Map {
    Tile** grid;
    int size;
} Map;

int loadTextures(SDL_Renderer* render);
int read_map_from_file(Map* map, char* file_name);
int display_map(SDL_Renderer* renderer, Map* map);
void destroy_map(Map* map);