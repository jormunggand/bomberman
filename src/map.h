#ifdef __linux__
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#elif __APPLE__
#include <SDL.h>
#include <SDL_image.h>
#endif

#include <stdbool.h>

typedef struct Bomb {
    SDL_Rect rect;
    int nb_ticks;
    int radius; // does not count the center tile
    bool detonated;
    bool* explosion_tiles; // array of size (2 * radius + 1)^2 to represent on which tiles the explosion must be displayed 
} Bomb;

typedef struct Tile {
    int type;
    Bomb* bomb;
} Tile;

typedef struct Map {
    Tile** grid;
    int size, starty, startx;
} Map;

int loadTextures(SDL_Renderer* render);
int read_map_from_file(Map* map, char* file_name);
int display_map(SDL_Renderer* renderer, Map* map);
void destroy_map(Map* map);