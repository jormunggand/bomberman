#include "utils.h"
#include "map.h"


int main(int argc, char* argv[]) {
    int exit_status = EXIT_FAILURE;

    int** map = malloc(MAP_SIZE * sizeof(int*));
    for (int i = 0; i < MAP_SIZE; i++) {
        map[i] = malloc(MAP_SIZE * sizeof(int));
    }
    
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    if (init(&window, &renderer, MAP_SIZE * TILE_SIZE, MAP_SIZE * TILE_SIZE) != 0) {
        printf("%s\n", SDL_GetError());
        goto Quit;
    }
    SDL_SetWindowTitle(window, "Map");

    map = read_map_from_file("map_example.txt", MAP_SIZE, MAP_SIZE);
    if (map == NULL) {
        printf("Error reading map from file\n");
        goto Quit;
    }
    //printf("%d\n", map[2][2]);
    if (display_map(renderer, map, MAP_SIZE, MAP_SIZE) != 0) {
        printf("Error displaying map\n");
        goto Quit;
    }

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
    // if (NULL != wall_texture)
    //     SDL_DestroyTexture(wall_texture);
    // if (NULL != empty_texture)
    //     SDL_DestroyTexture(empty_texture);
    // if (NULL != soft_wall_texture)
    //     SDL_DestroyTexture(soft_wall_texture);
    if(NULL != renderer)
        SDL_DestroyRenderer(renderer);
    if(NULL != window)
        SDL_DestroyWindow(window);
    SDL_Quit();
    destroy_map(map, MAP_SIZE, MAP_SIZE);
    return exit_status;
}