#include "utils.h"
#include "map.h"
#include "move.h"

int main(int argc, char* argv[]) {
    int exit_status = EXIT_FAILURE;

    SDL_Window* window = NULL;
    SDL_Renderer* render = NULL;
    SDL_Texture* player = NULL;
    int windowWidth = MAP_SIZE * TILE_SIZE, windowHeight = MAP_SIZE * TILE_SIZE;
    int** map = read_map_from_file("map_collision.txt", MAP_SIZE, MAP_SIZE);
    if (0 != init(&window, &render, windowWidth, windowHeight)) {
        printf("%s", SDL_GetError());
        goto Quit;
    }

    // load and display map
    display_map(render, map, MAP_SIZE, MAP_SIZE);

    // load and display player sprite
    player = loadImage("../assets/Bomberman/Front/Bman_F_f00.png", render);
    SDL_Rect playerRect = {.x = windowWidth/2, .y = windowHeight/2, .w = TILE_SIZE, .h = TILE_SIZE};
    SDL_QueryTexture(player, 0, 0, &playerRect.w, &playerRect.h);
    SDL_RenderCopy(render, player, NULL, &playerRect);
    
    SDL_RenderPresent(render);
    

    int velocity = 15;
    SDL_Event event;
    bool done = false;
    int vx = 0, vy = 0;
    while (!done) {
        SDL_WaitEvent(&event);
        if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_q))
            done = true;
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_DOWN)
                vy += velocity;
            else if (event.key.keysym.sym == SDLK_UP)
                vy -= velocity;
            else if (event.key.keysym.sym == SDLK_RIGHT)
                vx += velocity;
            else if (event.key.keysym.sym == SDLK_LEFT)
                vx -= velocity;

            edge_collision(window, &playerRect, map, vx, vy);
            vx = 0; vy = 0;
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