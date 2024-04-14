#include "utils.h"
#include "map.h"
#include "move.h"

int main(int argc, char* argv[]) {
    int exit_status = EXIT_FAILURE;

    SDL_Window* window = NULL;
    SDL_Renderer* render = NULL;
    int windowWidth = MAP_SIZE*TILE_SIZE, windowHeight = MAP_SIZE*TILE_SIZE;
    if (0 != init(&window, &render, windowWidth, windowHeight)) {
        printf("%s", SDL_GetError());
        goto Quit;
    }

    // load and display map
    int** map = read_map_from_file("map_example.txt", MAP_SIZE, MAP_SIZE);
    display_map(render, map, MAP_SIZE, MAP_SIZE);

    // load and display player sprite
    SDL_Texture* player = loadImage("../assets/Bomberman/Front/Bman_F_f00.png", render);
    SDL_Rect playerRect = {.x = windowWidth/2, .y = windowHeight/2, .w = 32, .h = 32};
    SDL_QueryTexture(player, 0, 0, &playerRect.w, &playerRect.h);
    SDL_RenderCopy(render, player, NULL, &playerRect);
    
    SDL_RenderPresent(render);
    

    int velocity = 40;
    SDL_Event event;
    bool done = false;
    while (!done) {
        SDL_WaitEvent(&event);
        if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_q))
            done = true;
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_DOWN)
                playerRect.y += velocity;
            else if (event.key.keysym.sym == SDLK_UP)
                playerRect.y -= velocity;
            else if (event.key.keysym.sym == SDLK_RIGHT)
                playerRect.x += velocity;
            else if (event.key.keysym.sym == SDLK_LEFT)
                playerRect.x -= velocity;

            edge_collision(window, &playerRect, map);

            SDL_RenderClear(render);
            display_map(render, map, MAP_SIZE, MAP_SIZE);
            SDL_RenderCopy(render, player, NULL, &playerRect);
            SDL_RenderPresent(render);
        }
    }


    exit_status = EXIT_SUCCESS;
//SDL_RenderClear(render);
Quit:
    SDL_DestroyTexture(player);
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();
    destroy_map(map, MAP_SIZE, MAP_SIZE);
    return exit_status;
}