#include "utils.h"
#include "map.h"
#include "move.h"

int main(int argc, char* argv[]) {
    int exit_status = EXIT_FAILURE;

    SDL_Window* window = NULL;
    SDL_Renderer* render = NULL;
    int windowWidth = MAP_SIZE * TILE_SIZE, windowHeight = MAP_SIZE * TILE_SIZE;
    Player player;
    init_player(&player, MAP_SIZE * TILE_SIZE/2, MAP_SIZE * TILE_SIZE/2);
    int** map = read_map_from_file("../maps/map_collision.txt", MAP_SIZE, MAP_SIZE);
    if (0 != init(&window, &render, windowWidth, windowHeight)) {
        printf("%s", SDL_GetError());
        goto Quit;
    }
    // SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

    // load and display map
    display_map(render, map, MAP_SIZE, MAP_SIZE);

    // load and display player sprite
    display_player(render, &player);

    SDL_RenderPresent(render);
    

    int velocity = 16;
    SDL_Event event;
    bool done = false;
    int cpt = 0, vx = 0, vy = 0;
    while (!done) {
        int eventPresent = SDL_PollEvent(&event);
        if (eventPresent) {
            player.isWalking = true;
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_q))
                done = true;
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_DOWN) {
                    vy += velocity;
                    change_direction(&player, FRONT);
                } else if (event.key.keysym.sym == SDLK_UP) {
                    vy -= velocity;
                    change_direction(&player, BACK);
                } else if (event.key.keysym.sym == SDLK_RIGHT) {
                    vx += velocity;
                    change_direction(&player, RIGHT);
                } else if (event.key.keysym.sym == SDLK_LEFT) {
                    vx -= velocity;
                    change_direction(&player, LEFT);
                }

                update_sprite(&player);
                edge_collision(window, &player, map, vx, vy);
                vx = 0; vy = 0;
            }
        }
        else{
            cpt++;
            if (cpt == 300){
                cpt = 0;
                player.isWalking = false;
            }
        }
        SDL_RenderClear(render);
        display_map(render, map, MAP_SIZE, MAP_SIZE);
        display_player(render, &player);
        SDL_RenderDrawRect(render, &player.collisionRect);
        SDL_RenderPresent(render);
    }


    exit_status = EXIT_SUCCESS;
Quit:
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();
    destroy_map(map, MAP_SIZE, MAP_SIZE);
    // destroy_player(&player);
    return exit_status;
}