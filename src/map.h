#include "utils.h"

// might be changed in the future
#define N_TEXTURES (5)
// the size of the board is currently fixed by defines but this will maybe be changed in the future to have variable sizes
#define MAP_SIZE (16)
#define TILE_SIZE (64)

typedef enum { // Tile types, will add the other types later
    HARD_WALL,
    EMPTY,
    SOFT_WALL
} TileType;

void loadTextures(SDL_Renderer* render);
int** read_map_from_file(char* file_name, int map_width, int map_height);
void display_map(SDL_Renderer* renderer, int** map, int map_width, int map_height);
void destroy_map(int** map, int map_width, int map_height);