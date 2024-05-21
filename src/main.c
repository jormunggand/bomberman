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

#ifndef HUD_H
#define HUD_H
#include "hud.h"
#endif

#ifndef ONLINE_H
#define ONLINE_H
#include "online.h"
#endif



#define SPLASH_SIZE (800)
#define BILLION (1000000000L)


// Load all textures required for the game in one place
// returns 0 if everything went well and -1 otherwise
int load_all_textures(SDL_Renderer* render) {
    int r1 = load_map_textures(render);
    int r2 = load_bomb_textures(render);
    // 0 for normal skin and 1 for negative colors
    int r3 = load_player_textures(render, 0);
    int r4 = load_player_textures(render, 1);
    int r5 = load_menu_textures(render);
    if (r1 + r2 + r3 + r4 + r5 != 0) {
        printf("%s\n", SDL_GetError());
        return -1;
    }
    return 0;
}


// Launch a game inside window with the renderer render where two players can play on the same keyboard S
// Gamemode is an enum defined in hud.h
Gamemode local_multiplayer(SDL_Window* window, SDL_Renderer* render, char* map_filename) {
    setColor(render, (SDL_Color){0, 0, 0});

    Map map;
    if (read_map_from_file(&map, map_filename) != 0)
    {
        printf("Error while opening the map file (%s)\n", SDL_GetError());
    }
    init_bonus(&map); // randomly add hidden bonuses in soft walls

    // Resize the window to fit the map
    SDL_SetWindowSize(window, map.size * TILE_SIZE, map.size * TILE_SIZE + HUD_HEIGHT);

    // Init the players
    const short nPlayers = 2;
    Player players[nPlayers];
    Key controls1[5] = {K_z, K_d, K_s, K_q, K_SPACE};
    Key controls2[5] = {K_UP, K_RIGHT, K_DOWN, K_LEFT, K_RSHIFT};
    init_player(&players[0], 1, 1, controls1, 0);
    init_player(&players[1], map.size-2, map.size-2, controls2, 1);

    Delta deltas[nPlayers]; // deltas are used to move the player sprites
    for (int i = 0; i < nPlayers; i++) {
        deltas[i].x = 0;
        deltas[i].y = 0;
    }

    // KeyboardHandler is defined in keyboard.h 
    // Enables simultaneous keypresses (moving diagonally for example)
    KeyboardHandler handler; 
    initHandler(&handler);

    // Load and display map and players
    display_map(render, &map);
    for (int i = 0; i < nPlayers; i++)
        display_player(render, &players[i]);
    SDL_RenderPresent(render);

    
    // Make sure the game doesn't run too fast or too slow on other machines
    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;
    double deltaTime = 0;
    double targetfps = 1.0 / 60.0;
    double accumulator = 0.0;

    // Code to display the timer
    char timerStr[8];
    SDL_Color White = {255, 255, 255};
    TTF_Font* sans = TTF_OpenFont("../assets/nasa.ttf", 12);
    if (sans == NULL) {
        printf("Error while loading font: %s\n", TTF_GetError());
        return QUIT;
    }
    struct timespec start_time;
    clock_gettime(CLOCK_REALTIME, &start_time);
    SDL_Rect timer_rect; //create a rect
    int ww, wh;
    SDL_GetWindowSize(window, &ww, &wh);
    timer_rect.w = 50; // controls the width of the rect
    timer_rect.h = 50; // controls the height of the rect
    timer_rect.y = 0; // controls the rect's y coordinte
    timer_rect.x = ww / 2 - timer_rect.w / 2;  //controls the rect's x coordinate 

    // Start the game loop
    SDL_Event event;
    bool done = false;
    while (!done) {
        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();
        deltaTime = (double)( (NOW - LAST) / (double) SDL_GetPerformanceFrequency() ); // delta time in seconds
        accumulator += deltaTime;

        int eventPresent = SDL_PollEvent(&event);
        if (eventPresent) {
            if (event.type == SDL_QUIT || handler.keyState[K_ESC] == SDL_PRESSED)
                done = true;
            else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
                handleEvent(&handler, event.key);
        }
        while (accumulator > targetfps) {
            for (int iPlayer = 0; iPlayer < nPlayers; iPlayer++) {
                if (!players[iPlayer].isAlive)
                    continue;
                Player* curPlayer = &players[iPlayer];
                if (anyDirectionPressed(&handler, iPlayer+1)) {
                    curPlayer->isWalking = true;
                    curPlayer->cpt_reset++;
                    update_sprite(curPlayer);

                    // reset the animation when the player is not moving
                    if (curPlayer->cpt_reset == 200) {
                        curPlayer->cpt_reset = 0;
                        curPlayer->isWalking = false;
                    }

                    // make the sprite move in the adequate direction
                    if (handler.keyState[curPlayer->controls[0]] == SDL_PRESSED) {
                        deltas[iPlayer].y -= 1;
                        change_direction(curPlayer, BACK, iPlayer);
                    }
                    if (handler.keyState[curPlayer->controls[1]] == SDL_PRESSED) {
                        deltas[iPlayer].x += 1;
                        change_direction(curPlayer, RIGHT, iPlayer);
                    }
                    if (handler.keyState[curPlayer->controls[2]] == SDL_PRESSED) {
                        deltas[iPlayer].y += 1;
                        change_direction(curPlayer, FRONT, iPlayer);
                    }
                    if (handler.keyState[curPlayer->controls[3]] == SDL_PRESSED) {
                        deltas[iPlayer].x -= 1;
                        change_direction(curPlayer, LEFT, iPlayer);
                    }
                }
                if (handler.keyState[curPlayer->controls[4]] == SDL_PRESSED)
                    player_place_bomb(curPlayer, &map);

                edge_collision(window, curPlayer, &map, deltas[iPlayer].x, deltas[iPlayer].y, targetfps);
                deltas[iPlayer].x = 0;
                deltas[iPlayer].y = 0;
                get_bonus(curPlayer, &map);
            }
            update_bombs_positions(window, &map, targetfps);
            accumulator -= targetfps;
        }
        // Disply the timer
        struct timespec cur_time;
        clock_gettime(CLOCK_REALTIME, &cur_time);
        double dt = (cur_time.tv_sec - start_time.tv_sec) + (double) (cur_time.tv_nsec - start_time.tv_nsec) / (double) BILLION;
        sprintf(timerStr, "%.2f", dt);
        SDL_Surface* surfaceMessage = TTF_RenderText_Solid(sans, timerStr, White); 
        SDL_Texture* message = SDL_CreateTextureFromSurface(render, surfaceMessage);
        
        // Display everything on screen
        SDL_RenderClear(render);
        display_map(render, &map);
        display_bombs(render, &map);
        for (int i = 0; i < nPlayers; i++) {
            updateDeathStatus(&map, &players[i]);
            if (players[i].isAlive) {display_player(render, &players[i]);}
        }
        SDL_RenderCopy(render, message, NULL, &timer_rect);
        display_hud(render, sans, &players[0], &players[1], ww);

        SDL_RenderPresent(render);

        if (map.nPlayersAlive <= 1) {
            return display_game_over(render, sans, players, ww, wh);
        }
    }
    return QUIT;
}



int main(int argc, char* argv[]) {
    int exit_status = EXIT_FAILURE;

    // Read the command line argument to load a specified map
    char map_filename[100];
    if (argc == 2) {
        strcpy(map_filename, argv[1]);
    } else {
        strcpy(map_filename,  "../maps/map_closed.txt"); // default map
    }
   
    // Create and init the SDL Window and Renderer
    SDL_Window* window = NULL;
    SDL_Renderer* render = NULL;
    int windowWidth = SPLASH_SIZE, windowHeight = SPLASH_SIZE;
    if (0 != init(&window, &render, windowWidth, windowHeight)) {
        printf("%s", SDL_GetError());
        goto Quit;
    }

    // Load all textures once and for all
    load_all_textures(render);

    // Start the menu
    Gamemode gamemode = choose_gamemode(render, windowWidth, windowHeight);
    while (gamemode != QUIT) {
        if (gamemode == LOCAL_MULTI) {
            gamemode = local_multiplayer(window, render, map_filename);
        } else if (gamemode == ONLINE_MENU) {
            gamemode = online_menu(render, windowWidth, windowHeight);
        } else if (gamemode == ONLINE_HOSTING) {
            host_server(window, render, map_filename);
            gamemode = CHOOSING;
        } else if (gamemode == ONLINE_JOINING) {
            join_server(window, render);
            gamemode = CHOOSING;
        } else if(gamemode == CHOOSING) {
            SDL_SetWindowSize(window, SPLASH_SIZE, SPLASH_SIZE);
            gamemode = choose_gamemode(render, windowWidth, windowHeight);
        } else if (gamemode == PvC) {
            gamemode = QUIT;
        }
    }

    exit_status = EXIT_SUCCESS;
    Quit:
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDLNet_Quit();
    TTF_Quit();
    SDL_Quit();
    return exit_status;
}