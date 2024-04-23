#ifdef __linux__
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#elif __APPLE__
#include <SDL.h>
#include <SDL_image.h>
#endif


struct Bomb;

typedef enum {
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

typedef struct Tile {
    TileType type;
    struct Bomb* bomb;
} Tile;

typedef struct Map {
    Tile** grid;
    int size, starty, startx;
} Map;


int loadTextures(SDL_Renderer* render);
int read_map_from_file(Map* map, char* file_name);
int display_map(SDL_Renderer* renderer, Map* map);
void destroy_map(Map* map);