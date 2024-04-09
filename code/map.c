#include "utils.h"


#define MAP_SIZE (16)
#define TILE_SIZE (64 / 2)

int main(int argc, char* argv[]) {
    int exit_status = EXIT_FAILURE;
    char* wall_tile = "wall_tile.png";
    char* floor_tile = "normal_tile.png";

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Texture* floor_texture = NULL; 
    SDL_Texture* wall_texture = NULL;

    if (init(&window, &renderer, MAP_SIZE * TILE_SIZE, MAP_SIZE * TILE_SIZE) != 0) {
        printf("%s\n", SDL_GetError());
        goto Quit;
    }
    SDL_SetWindowTitle(window, "Map");

    floor_texture = loadImage(floor_tile, renderer);
    wall_texture = loadImage(wall_tile, renderer);
    if (floor_texture == NULL || wall_texture == NULL) {
        printf("%s", SDL_GetError());
        goto Quit;
    }

    SDL_Rect rect = {0, 0, TILE_SIZE, TILE_SIZE};

    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            rect.x = i * TILE_SIZE;
            rect.y = j * TILE_SIZE;
            if (i == 0 || j == 0 || i == MAP_SIZE - 1 || j == MAP_SIZE - 1) {
                SDL_RenderCopy(renderer, wall_texture, NULL, &rect);
            } else {
                SDL_RenderCopy(renderer, floor_texture, NULL, &rect);
            }
        }
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
    Quit:
    if (NULL != wall_texture)
        SDL_DestroyTexture(wall_texture);
    if (NULL != floor_texture)
        SDL_DestroyTexture(floor_texture);
    if(NULL != renderer)
        SDL_DestroyRenderer(renderer);
    if(NULL != window)
        SDL_DestroyWindow(window);
    SDL_Quit();
    return exit_status;
}