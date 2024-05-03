#include "map.h"
#include "utils.h"

// Texture files
SDL_Texture* empty_texture = NULL; 
SDL_Texture* wall_texture = NULL;
SDL_Texture* soft_wall_texture = NULL;

SDL_Texture* bomb_bonus_icon = NULL;
SDL_Texture* flame_bonus_icon = NULL;
SDL_Texture* speed_bonus_icon = NULL;

SDL_Texture* bomb_bonus = NULL;
SDL_Texture* flame_bonus = NULL;
SDL_Texture* speed_bonus = NULL;

SDL_Texture* textures[N_TEXTURES];



// draw the bonus icon on top of the background tile

SDL_Texture* create_bonus_texture(SDL_Renderer* render, SDL_Texture* icon) {
    SDL_Texture* text = SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA8888, 
        SDL_TEXTUREACCESS_TARGET, TILE_SIZE, TILE_SIZE);
    SDL_SetRenderTarget(render, text);
    SDL_RenderCopy(render, empty_texture, NULL, NULL);
    SDL_Rect middle = {.x=TILE_SIZE/4, .y=TILE_SIZE/4, .w=TILE_SIZE/2, .h=TILE_SIZE/2};
    SDL_RenderCopy(render, icon, NULL, &middle);
    SDL_SetRenderTarget(render, NULL);
    return text;
}

int load_map_textures(SDL_Renderer* render) {
    empty_texture = loadImage("../assets/Blocks/BackgroundTile.png", render);
    wall_texture = loadImage("../assets/Blocks/SolidBlock.png", render);
    soft_wall_texture = loadImage("../assets/Blocks/ExplodableBlock.png", render);

    bomb_bonus_icon = loadImage("../assets/Powerups/BombPowerup.png", render);
    flame_bonus_icon = loadImage("../assets/Powerups/FlamePowerup.png", render);
    speed_bonus_icon = loadImage("../assets/Powerups/SpeedPowerup.png", render);

    bomb_bonus = create_bonus_texture(render, bomb_bonus_icon);
    flame_bonus = create_bonus_texture(render, flame_bonus_icon);
    speed_bonus = create_bonus_texture(render, speed_bonus_icon);
    

    if (empty_texture == NULL || wall_texture == NULL || soft_wall_texture == NULL
        || bomb_bonus_icon == NULL || flame_bonus_icon == NULL || speed_bonus_icon == NULL
        || bomb_bonus == NULL || flame_bonus == NULL || speed_bonus == NULL) {
        printf("%s\n", SDL_GetError());
        return -1;
    }

    // now useless
    SDL_DestroyTexture(bomb_bonus_icon);
    SDL_DestroyTexture(flame_bonus_icon);
    SDL_DestroyTexture(speed_bonus_icon);

    textures[HARD_WALL] = wall_texture;
    textures[EMPTY] = empty_texture;
    textures[SOFT_WALL] = soft_wall_texture;

    textures[PLAYER_SPAWN] = empty_texture;

    textures[BOMB_BONUS] = bomb_bonus;
    textures[SPEED_BONUS] = speed_bonus;
    textures[FLAME_BONUS] = flame_bonus;

    
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

    map->grid = malloc(size * sizeof(Tile*));
    for (int y = 0; y < size; y++) {
        map->grid[y] = malloc(size * sizeof(Tile));
        for (int x = 0; x < size; x++) {
            map->grid[y][x].type = fgetc(file) - '0';
            map->grid[y][x].bonus = NONE;
            map->grid[y][x].bomb = NULL;
            if (map->grid[y][x].type == PLAYER_SPAWN) {
                map->starty = y;
                map->startx = x;
            }
        }
        fgetc(file); // to skip the newline character
    }
    fclose(file);

    return 0;
}


// a function to update the renderer given the current state of the map
int display_map(SDL_Renderer* renderer, Map* map) {
    SDL_Rect rect = {0, 0, TILE_SIZE, TILE_SIZE};
    for (int y = 0; y < map->size; y++) {
        for (int x = 0; x < map->size; x++) {
            rect.x = x * TILE_SIZE;
            rect.y = y * TILE_SIZE;
            int r;
            if (map->grid[y][x].bonus != NONE && map->grid[y][x].type == EMPTY){
                r = SDL_RenderCopy(renderer, textures[map->grid[y][x].bonus], NULL, &rect);
            }
            else{
                r = SDL_RenderCopy(renderer, textures[map->grid[y][x].type], NULL, &rect);
            } 
            if (r != 0) {
                printf("%s\n", SDL_GetError());
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
