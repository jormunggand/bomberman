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


int main(int argc, char* argv[]) {
    int exit_status = EXIT_FAILURE;

    Map map;
    if (argc == 2) {
        if (read_map_from_file(&map, argv[1]) == -1) {
            printf("Error while opening the map file - Usage: %s FILE\n", argv[0]);
            return exit_status;
        } 
    } else {
        read_map_from_file(&map, "../maps/map_collision.txt");
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
    init_player(&player, map.starty, map.startx);

    // load and display map and player
    display_map(render, &map);
    display_player(render, &player);

    SDL_RenderPresent(render);
    

    SDL_Event event;
    KeyboardHandler handler;
    initHandler(&handler);

    bool done = false;
    int cpt_reset = 0;
    float vx = 0, vy = 0;

    while (!done) {
        int eventPresent = SDL_PollEvent(&event);
        if (eventPresent) {
            if (event.type == SDL_QUIT || handler.keyState[K_ESC] == SDL_PRESSED)
                done = true;
            else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
                handleEvent(&handler, event.key);
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                int i = x / TILE_SIZE, j = y / TILE_SIZE;
                add_bomb(&map, i, j);
            }


        if (handler.keyState[K_DOWN] == SDL_PRESSED) {
            player.isWalking = true;
            vy += player.speed;
            change_direction(&player, FRONT);
        } else if (handler.keyState[K_UP] == SDL_PRESSED) {
            player.isWalking = true;
            vy -= player.speed;
            change_direction(&player, BACK);
        } else if (handler.keyState[K_RIGHT] == SDL_PRESSED) {
            player.isWalking = true;
            vx += player.speed;
            change_direction(&player, RIGHT);
            
        } else if (handler.keyState[K_LEFT] == SDL_PRESSED) {
            player.isWalking = true;
            vx -= player.speed;
            change_direction(&player, LEFT);
        }
        else{
            cpt_reset++;
            if (cpt_reset == 200){
                cpt_reset = 0;
                player.isWalking = false;
            }
        }

        if (handler.keyState[K_SPACE] == SDL_PRESSED) {
            player_place_bomb(&player, &map);
        }

        update_sprite(&player);
        edge_collision(window, &player, &map, vx, vy);
        vx = 0; vy = 0;
        get_bonus(&player, &map);
    }

        
        SDL_RenderClear(render);
        display_map(render, &map);
        display_bombs(render, &map);
        display_player(render, &player);
        //SDL_RenderDrawRect(render, &player.rect);
        //SDL_RenderDrawRect(render, &player.collisionRect);
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