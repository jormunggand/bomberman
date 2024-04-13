#include "utils.h"

// the size of the board is currently fixed by defines but this will maybe be changed in the future to have variable sizes
#define MAP_SIZE (16)
#define TILE_SIZE (32)

typedef enum { // Tile types, will add the other types later
    HARD_WALL,
    EMPTY,
    SOFT_WALL
} TileType;

void read_map_from_file(int** map, char* file_name);
void update_renderer(SDL_Renderer* renderer, int** map, SDL_Texture* textures[], int nb_textures);
