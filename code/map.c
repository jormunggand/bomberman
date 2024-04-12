#include "utils.h"

// the size of the board is currently fixed by defines but this will maybe be changed in the future to have variable sizes
#define MAP_SIZE (16)
#define TILE_SIZE (32)

typedef enum { // Tile types, will add the other types later
    HARD_WALL,
    EMPTY,
    SOFT_WALL
} TileType;


// function to load the map from a text file
void read_map_from_file(int** map, char* file_name) {
    FILE* file = fopen(file_name, "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return;
    }
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            map[i][j] = fgetc(file) - '0';
        }
        fgetc(file); // to skip the newline character
    }
    fclose(file);
}

// a function to update the renderer given the current state of the map
void update_renderer(SDL_Renderer* renderer, int** map, SDL_Texture* textures[], int nb_textures) {
    SDL_Rect rect = {0, 0, TILE_SIZE, TILE_SIZE};
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            rect.x = i * TILE_SIZE;
            rect.y = j * TILE_SIZE;
            SDL_RenderCopy(renderer, textures[map[i][j]], NULL, &rect);
        }
    }
}
