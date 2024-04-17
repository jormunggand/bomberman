#include "utils.h"
#include "map.h"
#include "move.h"

int main(int argc, char* argv[]) {
    int exit_status = EXIT_FAILURE;

    Map map;
    if (argc == 2) {
        if (read_map_from_file(&map, argv[1]) == -1) {
            printf("Error while opening the map file - Usage: %s FILE\n", argv[0]);
            return exit_status;
        } 
    } else {
        read_map_from_file(&map, "../maps/map_example.txt");
    }


    SDL_Window* window = NULL;
    SDL_Renderer* render = NULL;
    int windowWidth = map.size * TILE_SIZE, windowHeight = map.size * TILE_SIZE;
    if (0 != init(&window, &render, windowWidth, windowHeight)) {
        printf("%s", SDL_GetError());
        goto Quit;
    }
    // SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

    Player player;
    //printf("%d %d\n", map.starty, map.startx);
    init_player(&player, map.starty, map.startx);

    // load and display map and player
    display_map(render, &map);
    display_player(render, &player);

    SDL_RenderPresent(render);
    

    SDL_Event event;
    bool done = false;
    int cpt = 0, vx = 0, vy = 0;
    while (!done) {
        int eventPresent = SDL_PollEvent(&event);
        if (eventPresent) {
            player.isWalking = true;
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
                done = true;
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s) {
                    vy += player.speed;
                    change_direction(&player, FRONT);
                } else if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_z) {
                    vy -= player.speed;
                    change_direction(&player, BACK);
                } else if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d) {
                    vx += player.speed;
                    change_direction(&player, RIGHT);
                } else if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_q) {
                    vx -= player.speed;
                    change_direction(&player, LEFT);
                }

                update_sprite(&player);
                edge_collision(window, &player, &map, vx, vy);
                vx = 0; vy = 0;
            }
        }
        else{
            cpt++;
            if (cpt == 200){
                cpt = 0;
                player.isWalking = false;
            }
        }
        SDL_RenderClear(render);
        display_map(render, &map);
        display_player(render, &player);
        SDL_RenderDrawRect(render, &player.rect);
        SDL_RenderDrawRect(render, &player.collisionRect);
        SDL_RenderPresent(render);
    }


    exit_status = EXIT_SUCCESS;
Quit:
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();
    destroy_map(&map);
    // destroy_player(&player);
    return exit_status;
}