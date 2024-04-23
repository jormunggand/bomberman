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
    BOMB_BONUS = 4,
    SPEED_BONUS = 5,
    FLAME_BONUS = 6
} TileType;

typedef struct Tile {
    TileType type;
    struct Bomb* bomb;
} Tile;

typedef struct Map {
    Tile** grid;
    int size, starty, startx;
} Map;

SDL_Texture* create_bonus_texture(SDL_Renderer* render, SDL_Texture* icon);
int load_map_textures(SDL_Renderer* render);
int read_map_from_file(Map* map, char* file_name);
int display_map(SDL_Renderer* renderer, Map* map);
void destroy_map(Map* map);