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
    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

    // load and display map
    int** map = read_map_from_file("map_example.txt", MAP_SIZE, MAP_SIZE);
    display_map(render, map, MAP_SIZE, MAP_SIZE);

    // load and display player sprite
    Player* player = create_player(MAP_SIZE*TILE_SIZE/2, MAP_SIZE*TILE_SIZE/2);
    display_player(render, player);

    SDL_RenderPresent(render);
    

    int velocity = 15;
    SDL_Event event;
    bool done = false;
    while (!done) {
        int eventPresent = SDL_PollEvent(&event);
        if (eventPresent) {
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_q))
                done = true;
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_DOWN) {
                    player->rect->y += velocity;
                    change_direction(player, FRONT);
                } else if (event.key.keysym.sym == SDLK_UP) {
                    player->rect->y -= velocity;
                    change_direction(player, BACK);
                } else if (event.key.keysym.sym == SDLK_RIGHT) {
                    player->rect->x += velocity;
                    change_direction(player, RIGHT);
                } else if (event.key.keysym.sym == SDLK_LEFT) {
                    player->rect->x -= velocity;
                    change_direction(player, LEFT);
                }

                update_sprite(player);
                //edge_collision(window, &playerRect, map);
            }
        }
        SDL_RenderClear(render);
        display_map(render, map, MAP_SIZE, MAP_SIZE);
        display_player(render, player);
        SDL_RenderPresent(render);
    }


    exit_status = EXIT_SUCCESS;
//SDL_RenderClear(render);
Quit:
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();
    destroy_map(map, MAP_SIZE, MAP_SIZE);
    destroy_player(player);
    return exit_status;
}