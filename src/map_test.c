#include "utils.h"
#include "map.h"

int main(int argc, char* argv[]) {
    int exit_status = EXIT_FAILURE;

    int** map = malloc(MAP_SIZE * sizeof(int*));
    for (int i = 0; i < MAP_SIZE; i++) {
        map[i] = malloc(MAP_SIZE * sizeof(int));
    }
    
    char* wall_tile = "../assets/Blocks/SolidBlock.png";
    char* empty_tile = "../assets/Blocks/BackgroundTile.png";
    char* soft_wall_tile = "../assets/Blocks/ExplodableBlock.png";

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Texture* empty_texture = NULL; 
    SDL_Texture* wall_texture = NULL;
    SDL_Texture* soft_wall_texture = NULL;

    if (init(&window, &renderer, MAP_SIZE * TILE_SIZE, MAP_SIZE * TILE_SIZE) != 0) {
        printf("%s\n", SDL_GetError());
        goto Quit;
    }
    SDL_SetWindowTitle(window, "Map");

    empty_texture = loadImage(empty_tile, renderer);
    wall_texture = loadImage(wall_tile, renderer);
    soft_wall_texture = loadImage(soft_wall_tile, renderer);
    if (empty_texture == NULL || wall_texture == NULL || soft_wall_texture == NULL) {
        printf("%s", SDL_GetError());
        goto Quit;
    }

    SDL_Texture* textures[] = {wall_texture, empty_texture, soft_wall_texture};
    read_map_from_file(map, "map_example.txt");
    //printf("%d\n", map[2][2]);
    update_renderer(renderer, map, textures, 3);

    SDL_RenderPresent(renderer);
    bool is_running = true;
    SDL_Event event;
    while (is_running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                is_running = false;
            }
        }
        SDL_Delay(16);
    }

    exit_status = EXIT_SUCCESS;
    Quit:
    if (NULL != wall_texture)
        SDL_DestroyTexture(wall_texture);
    if (NULL != empty_texture)
        SDL_DestroyTexture(empty_texture);
    if (NULL != soft_wall_texture)
        SDL_DestroyTexture(soft_wall_texture);
    if(NULL != renderer)
        SDL_DestroyRenderer(renderer);
    if(NULL != window)
        SDL_DestroyWindow(window);
    SDL_Quit();
    for (int i = 0; i < MAP_SIZE; i++) {
        free(map[i]);
    }
    free(map);
    return exit_status;
}