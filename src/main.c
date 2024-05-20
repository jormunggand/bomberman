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


int load_all_textures(SDL_Renderer* render) {
    int r1 = load_map_textures(render);
    int r2 = load_bomb_textures(render);
    int r3 = load_player_textures(render);
    int r4 = load_menu_textures(render);
    if (r1 + r2 + r3 + r4 != 0) {
        printf("%s\n", SDL_GetError());
        return -1;
    }
    return 0;
}

/*void play_game(SDL_Window* window, SDL_Renderer* render, char* map_filename) {
    Map map;
    if (read_map_from_file(&map, map_filename) != 0)
    {
        printf("Error while opening the map file (%s)\n", SDL_GetError());
    }
    init_bonus(&map); // randomly add hidden bonuses in soft walls

    // Resize the window to fit the map
    SDL_SetWindowSize(window, map.size * TILE_SIZE, map.size * TILE_SIZE);

    Player player;
    init_player(&player, map.starty, map.startx);

    // load and display map and player
    load_all_textures(render);
    display_map(render, &map);
    display_player(render, &player);

    SDL_RenderPresent(render);
    
    SDL_Event event;
    KeyboardHandler handler; // to handle simultneous keypresses
    initHandler(&handler);

    bool done = false, draw_hitboxes = false;
    int cpt_reset = 0;
    int deltaX = 0, deltaY = 0;

    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;
    double deltaTime = 0;
    double targetfps = 1.0 / 60.0;
    double accumulator = 0.0;


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
            if (player.isAlive) {
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
                edge_collision(window, &player, &map, deltaX, deltaY, targetfps);
            
                deltaX = 0; deltaY = 0;
                get_bonus(&player, &map);
            }

            accumulator -= targetfps;
        }
        
        SDL_RenderClear(render);
        display_map(render, &map);
        display_bombs(render, &map, &player);
        if (player.isAlive) {display_player(render, &player);}
        if (draw_hitboxes) {
            SDL_RenderDrawRect(render, &player.rect);
            SDL_RenderDrawRect(render, &player.flameHitbox);
            SDL_RenderDrawRect(render, &player.collisionRect);
        }
        SDL_RenderPresent(render);
    }

}*/

// launch a game with two players
Gamemode local_multiplayer(SDL_Window* window, SDL_Renderer* render, char* map_filename) {
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
    init_player(&players[0], 1, 1, controls1);
    init_player(&players[1], map.size-2, map.size-2, controls2);

    // Load and display map and players
    display_map(render, &map);
    for (int i = 0; i < nPlayers; i++)
        display_player(render, &players[i]);
    SDL_RenderPresent(render);
    
    SDL_Event event;
    KeyboardHandler handler; // to handle simultaneous keypresses
    initHandler(&handler);

    bool done = false;
    Delta deltas[nPlayers];
    for (int i = 0; i < nPlayers; i++) {
        deltas[i].x = 0;
        deltas[i].y = 0;
    }
    printf("%d\n", deltas[0].x);

    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;
    double deltaTime = 0;
    double targetfps = 1.0 / 60.0;
    double accumulator = 0.0;

    char timerStr[8];
    SDL_Color White = {255, 255, 255};
    SDL_Color Black = {0, 0, 0};
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
                    if (curPlayer->cpt_reset == 200) {
                        curPlayer->cpt_reset = 0;
                        curPlayer->isWalking = false;
                    }

                    if (handler.keyState[curPlayer->controls[0]] == SDL_PRESSED) {
                        deltas[iPlayer].y -= 1;
                        change_direction(curPlayer, BACK);
                    }
                    if (handler.keyState[curPlayer->controls[1]] == SDL_PRESSED) {
                        deltas[iPlayer].x += 1;
                        change_direction(curPlayer, RIGHT);
                    }
                    if (handler.keyState[curPlayer->controls[2]] == SDL_PRESSED) {
                        deltas[iPlayer].y += 1;
                        change_direction(curPlayer, FRONT);
                    }
                    if (handler.keyState[curPlayer->controls[3]] == SDL_PRESSED) {
                        deltas[iPlayer].x -= 1;
                        change_direction(curPlayer, LEFT);
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
        struct timespec cur_time;
        clock_gettime(CLOCK_REALTIME, &cur_time);
        double dt = (cur_time.tv_sec - start_time.tv_sec) + (double) (cur_time.tv_nsec - start_time.tv_nsec) / (double) BILLION;
        sprintf(timerStr, "%.2f", dt);
        SDL_Surface* surfaceMessage = TTF_RenderText_Solid(sans, timerStr, White); 
        SDL_Texture* message = SDL_CreateTextureFromSurface(render, surfaceMessage);
        
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
            int pitch = sizeof(Uint32) * ww;
            Uint32 *pixels = malloc(pitch * wh);
            SDL_RenderReadPixels(render, NULL, SDL_PIXELFORMAT_RGBA8888, pixels, pitch);
            // gaussian blur on the screen
            gaussian_blur(pixels, ww, wh);
            SDL_Texture* texture = SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, ww, wh);
            SDL_UpdateTexture(texture, NULL, pixels, pitch);
            SDL_RenderClear(render);
            renderTexture(render, texture, NULL, NULL);
            free(pixels);

            SDL_Rect game_over_rect;
            game_over_rect.w = 200;
            game_over_rect.h = 120;
            game_over_rect.x = ww / 2 - game_over_rect.w / 2;
            game_over_rect.y = wh / 2 - game_over_rect.h / 2 - 100;
            char game_over_str[20];
             if (players[0].isAlive)
                sprintf(game_over_str, "Player 1 wins");
            else if (players[1].isAlive)
                sprintf(game_over_str, "Player 2 wins");
            else 
                sprintf(game_over_str, "Draw");
            SDL_Surface* surface_go = TTF_RenderText_Solid(sans, game_over_str, Black); 
            SDL_Texture* mess_go = SDL_CreateTextureFromSurface(render, surface_go);
            SDL_RenderCopy(render, mess_go, NULL, &game_over_rect);

            SDL_Rect choice_rect;
            choice_rect.w = 200;
            choice_rect.h = 120;
            choice_rect.x = ww / 2 - choice_rect.w / 2;
            choice_rect.y = wh / 2 - choice_rect.h / 2 + 100;
            char choice_str[20];
            sprintf(choice_str, "Play again?");
            SDL_Surface* surface_choice = TTF_RenderText_Solid(sans, choice_str, Black);
            SDL_Texture* mess_choice = SDL_CreateTextureFromSurface(render, surface_choice);
            SDL_RenderCopy(render, mess_choice, NULL, &choice_rect);

            SDL_Rect yes_rect;
            yes_rect.w = 50;
            yes_rect.h = 50;
            yes_rect.x = ww / 2 - yes_rect.w / 2 - 100;
            yes_rect.y = wh / 2 - yes_rect.h / 2 + 200;
            char yes_str[20];
            sprintf(yes_str, "Yes");
            SDL_Surface* surface_yes = TTF_RenderText_Solid(sans, yes_str, Black);
            SDL_Texture* mess_yes = SDL_CreateTextureFromSurface(render, surface_yes);
            SDL_RenderCopy(render, mess_yes, NULL, &yes_rect);

            SDL_Rect no_rect;
            no_rect.w = 50;
            no_rect.h = 50;
            no_rect.x = ww / 2 - no_rect.w / 2 + 100;
            no_rect.y = wh / 2 - no_rect.h / 2 + 200;
            char no_str[20];
            sprintf(no_str, "No");
            SDL_Surface* surface_no = TTF_RenderText_Solid(sans, no_str, Black);
            SDL_Texture* mess_no = SDL_CreateTextureFromSurface(render, surface_no);
            SDL_RenderCopy(render, mess_no, NULL, &no_rect);
            SDL_RenderPresent(render);

            free_bombs();

            SDL_Event event;
            while (1) {
                if (SDL_WaitEvent(&event)) {
                    if (event.type == SDL_QUIT) {
                        return QUIT;
                    }
                    if (event.type == SDL_MOUSEBUTTONDOWN) {
                        int x = event.button.x;
                        int y = event.button.y;
                        if (point_in_rect(yes_rect, x, y)) {
                            return LOCAL_MULTI;
                        } else if (point_in_rect(no_rect, x, y)) {
                            return CHOOSING;
                        }
                    }
                }
            }
        }
    }
    return QUIT;
}



int main(int argc, char* argv[]) {
    int exit_status = EXIT_FAILURE;

    // Read the command line argument
    char map_filename[100];
    if (argc == 2) {
        strcpy(map_filename, argv[1]);
    } else {
        strcpy(map_filename,  "../maps/map_bonus.txt"); // default map
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

    // Start the game
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
            join_server(render, windowWidth, windowHeight);
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
    SDL_Quit();
    return exit_status;
}