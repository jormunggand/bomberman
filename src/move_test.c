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
    SDL_Rect playerRect = {.x = TILE_SIZE * MAP_SIZE/2, .y = TILE_SIZE * MAP_SIZE/2, .w = TILE_SIZE, .h = 2 * TILE_SIZE};
    SDL_QueryTexture(player, 0, 0, &playerRect.w, &playerRect.h);
    SDL_Rect collision_rect = {.x = TILE_SIZE * MAP_SIZE/2, .y = TILE_SIZE * MAP_SIZE/2 + 5 * playerRect.h/ 8, .w = playerRect.w, .h = playerRect.h/2};
    SDL_RenderCopy(render, player, NULL, &playerRect);
    
    SDL_RenderPresent(render);
    

    int velocity = 16;
    SDL_Event event;
    bool done = false;
    int vx = 0, vy = 0;
    while (!done) {
        SDL_WaitEvent(&event);
        if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
            done = true;
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
                vy += velocity;
            else if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_z)
                vy -= velocity;
            else if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
                vx += velocity;
            else if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_q)
                vx -= velocity;

            edge_collision(window, &playerRect, &collision_rect, map, vx, vy);
            vx = 0; vy = 0;
            SDL_RenderClear(render);
            display_map(render, map, MAP_SIZE, MAP_SIZE);
            SDL_RenderDrawRect(render, &collision_rect);   
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