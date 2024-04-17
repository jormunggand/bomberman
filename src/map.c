#include "map.h"
#include "utils.h"

// keep track of whether or not the textures have been loaded
bool loadedTiles = false;

// Texture files
SDL_Texture* empty_texture = NULL; 
SDL_Texture* wall_texture = NULL;
SDL_Texture* soft_wall_texture = NULL;

SDL_Texture* textures[N_TEXTURES];


int loadTextures(SDL_Renderer* render) {
    empty_texture = loadImage("../assets/Blocks/BackgroundTile.png", render);
    wall_texture = loadImage("../assets/Blocks/SolidBlock.png", render);
    soft_wall_texture = loadImage("../assets/Blocks/ExplodableBlock.png", render);
    if (empty_texture == NULL || wall_texture == NULL || soft_wall_texture == NULL) {
        printf("%s", SDL_GetError());
        return -1;
    }

    textures[0] = wall_texture;
    textures[1] = empty_texture;
    textures[2] = soft_wall_texture;

    return 0;
}


// function to load the map from a text file
int read_map_from_file(Map* map, char* file_name) {
    FILE* file = fopen(file_name, "r");
    if (file == NULL) {
        printf("Error opening %s\n", file_name);
        return -1;
    }

    int size;
    fscanf(file, "%d\n", &size);
    map->size = size;

    map->grid = malloc(size*sizeof(int*));
    for (int y = 0; y < size; y++) {
        map->grid[y] = malloc(size*sizeof(int));
        for (int x = 0; x < size; x++) {
            map->grid[y][x] = fgetc(file) - '0';
        }
        fgetc(file); // to skip the newline character
    }
    fclose(file);

    return 0;
}


// a function to update the renderer given the current state of the map
int display_map(SDL_Renderer* renderer, Map* map) {
    if (!loadedTiles) {
        loadTextures(renderer);
        loadedTiles = true;
    }

    SDL_Rect rect = {0, 0, TILE_SIZE, TILE_SIZE};
    for (int y = 0; y < map->size; y++) {
        for (int x = 0; x < map->size; x++) {
            rect.x = x * TILE_SIZE;
            rect.y = y * TILE_SIZE;
            if (SDL_RenderCopy(renderer, textures[map->grid[y][x]], NULL, &rect) != 0){
                printf("%s", SDL_GetError());
                return -1;
            }
        }
    }
    return 0;
}


// destroy memory allocated for the map
void destroy_map(Map* map) {
    for (int y = 0; y < map->size; y++)
        free(map->grid[y]);
    free(map->grid);
}
