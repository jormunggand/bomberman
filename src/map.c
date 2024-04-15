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
int** read_map_from_file(char* file_name, int map_width, int map_height) {
    FILE* file = fopen(file_name, "r");
    if (file == NULL) {
        printf("Error opening %s\n", file_name);
        return NULL;
    }

    int** map = malloc(map_height*sizeof(int*));
    for (int y = 0; y < map_height; y++)
        map[y] = malloc(map_width*sizeof(int));
    
    for (int y = 0; y < map_height; y++) {
        for (int x = 0; x < map_width; x++) {
            map[y][x] = fgetc(file) - '0';
        }
        fgetc(file); // to skip the newline character
    }
    fclose(file);
    return map;
}


// a function to update the renderer given the current state of the map
void display_map(SDL_Renderer* renderer, int** map, int map_width, int map_height) {
    if (!loadedTiles) {
        loadTextures(renderer);
        loadedTiles = true;
    }

    SDL_Rect rect = {0, 0, TILE_SIZE, TILE_SIZE};
    for (int y = 0; y < map_height; y++) {
        for (int x = 0; x < map_width; x++) {
            rect.x = x * TILE_SIZE;
            rect.y = y * TILE_SIZE;
            if (SDL_RenderCopy(renderer, textures[map[y][x]], NULL, &rect) != 0){
                printf("%s", SDL_GetError());
                return -1;
            }
        }
    }
    return 0;
}


// destroy memory allocated for the map
void destroy_map(int** map, int map_width, int map_height) {
    for (int y = 0; y < map_height; y++)
        free(map[y]);
    free(map);
}
