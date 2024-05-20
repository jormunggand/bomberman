#ifdef __linux__
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#elif __APPLE__
#include <SDL.h>
#include <SDL_image.h>
#endif

extern SDL_Texture* bomb_bonus_icon;
extern SDL_Texture* flame_bonus_icon;
extern SDL_Texture* speed_bonus_icon;

struct Bomb;

typedef enum {
    HARD_WALL = 0,
    EMPTY = 1,
    SOFT_WALL = 2,
    PLAYER_SPAWN = 3,
} TileType;

typedef enum {
    BOMB_BONUS = 4,
    SPEED_BONUS = 5,
    FLAME_BONUS = 6,
    NONE = 7,
} BonusType;

typedef struct Tile {
    TileType type; // currently shown texture
    BonusType bonus; // bonus to be displayed when a SOFT_WALL is broken
} Tile;

typedef struct Map {
    Tile** grid;
    int size, starty, startx;
    int nPlayersAlive; // number of players alive on the map
} Map;

// create a bonus texture that will be displayed on the map by combining the empty tile texture and the bonus texture
SDL_Texture* create_bonus_texture(SDL_Renderer* render, SDL_Texture* icon);

// load the textures for the map
int load_map_textures(SDL_Renderer* render);

// load the map from a text file
int read_map_from_file(Map* map, char* file_name);

// display the map on the screen
int display_map(SDL_Renderer* renderer, Map* map);

// destroy the map and free the memory
void destroy_map(Map* map);