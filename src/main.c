#include <time.h>
#include <unistd.h>

#include "utils.h"
#include "keyboard.h"

#ifndef MAP_H
#define MAP_H
#include "map.h"
#endif

#ifndef BOMB_H
#define BOMB_H
#include "bomb.h"
#endif

#ifndef MOVE_H
#define MOVE_H
#include "player.h"
#endif

#ifndef BONUS_H
#define BONUS_H
#include "bonus.h"
#endif


int load_all_textures(SDL_Renderer* render) {
    int r1 = load_map_textures(render);
    int r2 = load_bomb_textures(render);
    int r3 = load_player_textures(render);
    if (r1 + r2 + r3 != 0) {
        printf("%s\n", SDL_GetError());
        return -1;
    }
    return 0;
}

int main(int argc, char* argv[]) {
    int exit_status = EXIT_FAILURE;

    // Read a map using the command line argument or load a default map
    Map map;
    if (argc == 2) {
        if (read_map_from_file(&map, argv[1]) == -1) {
            printf("Error while opening the map file - Usage: %s FILE\n", argv[0]);
            return exit_status;
        } 
    } else {
        read_map_from_file(&map, "../maps/map_collision.txt");
    }
    // Randomly place powerups in the soft walls of the map
    init_bonus(&map);


    SDL_Window* window = NULL;
    SDL_Renderer* render = NULL;
    int windowWidth = map.size * TILE_SIZE, windowHeight = map.size * TILE_SIZE;
    if (0 != init(&window, &render, windowWidth, windowHeight)) {
        printf("%s", SDL_GetError());
        goto Quit;
    }

    Player player;
    init_player(&player, map.starty, map.startx);

    // load and display map and player
    load_all_textures(render);
    display_map(render, &map);
    display_player(render, &player);

    SDL_RenderPresent(render);
    

    SDL_Event event;
    KeyboardHandler handler; // to handle multiple keypresses
    initHandler(&handler);

    bool done = false, draw_hitboxes = false;
    int cpt_reset = 0;
    int deltaX = 0, deltaY = 0;

    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;
    double deltaTime = 0;
    double targetfps = 1.0 / 60.0;
    double accumulator = 0.0;

    // int cpt = 0;

    while (!done) {
        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();
        deltaTime = (double)( (NOW - LAST) / (double) SDL_GetPerformanceFrequency() ); // delta time in seconds
        accumulator += deltaTime;

        int eventPresent = SDL_PollEvent(&event);
        if (eventPresent) {
            if (event.type == SDL_QUIT || handler.keyState[K_ESC] == SDL_PRESSED)
                done = true;
            else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
                handleEvent(&handler, event.key);
            }
        }
        while (accumulator > targetfps)
        {
            // cpt++;
            if (anyDirectionPressed(&handler)) {
                player.isWalking = true;
                cpt_reset++;
                update_sprite(&player);
                if (cpt_reset == 200){
                    cpt_reset = 0;
                    player.isWalking = false;
                }
                if (handler.keyState[K_DOWN] == SDL_PRESSED) {
                    deltaY += 1;
                    change_direction(&player, FRONT);
                }
                if (handler.keyState[K_UP] == SDL_PRESSED) {
                    deltaY -= 1;
                    change_direction(&player, BACK);
                }
                if (handler.keyState[K_RIGHT] == SDL_PRESSED) {
                    deltaX += 1;
                    change_direction(&player, RIGHT);
                } 
                if (handler.keyState[K_LEFT] == SDL_PRESSED) {
                    deltaX -= 1;
                    change_direction(&player, LEFT);
                }
            }

            if (handler.keyState[K_SPACE] == SDL_PRESSED) {
                player_place_bomb(&player, &map);
            }
            // printf("%lf\n", elapsedTime);
            edge_collision(window, &player, &map, deltaX, deltaY, targetfps);
        
            deltaX = 0; deltaY = 0;
            get_bonus(&player, &map);

            accumulator -= targetfps;
        }
        // printf("nb while  = %d\n", cpt); 
        // cpt = 0; 
        
        SDL_RenderClear(render);
        display_map(render, &map);
        display_bombs(render, &map);
        display_player(render, &player);
        if (draw_hitboxes) {
            SDL_RenderDrawRect(render, &player.rect);
            SDL_RenderDrawRect(render, &player.collisionRect);
        }
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