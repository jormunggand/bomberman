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

#define SPLASH_SIZE (800)
#define BILLION (1000000000L)

typedef enum {
    LOCAL_MULTI,
    ONLINE_MULTI,
    PvC,
    CHOOSING, // the player has not chosed yet
    QUIT  // the player has closed the window
} Gamemode;

SDL_Texture* splashscreen;
SDL_Texture* local_multi_btn;
SDL_Texture* online_multi_btn;
SDL_Texture* playervcpu_btn;

// load textures needed for the booting menu
int load_menu_textures(SDL_Renderer* render) {
    splashscreen = loadImage("../assets/title_flat.jpg", render);
    local_multi_btn = loadImage("../assets/Menu/local_multiplayer_button.png", render);
    online_multi_btn = loadImage("../assets/Menu/online_multiplayer_button.png", render);
    playervcpu_btn = loadImage("../assets/Menu/player_vs_cpu_button.png", render);

    if (splashscreen == NULL || local_multi_btn == NULL || online_multi_btn == NULL || playervcpu_btn == NULL) {
        printf("%s\n", SDL_GetError());
        return -1;      
    }
    return 0;
}


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

// display the screen where the player can choose the gamemode
Gamemode choose_gamemode(SDL_Renderer* render, int windowWidth, int windowHeight) {
    // display the background and the buttons
    if (load_menu_textures(render) != 0) {
        printf("%s\n", SDL_GetError());
        return QUIT;
    }
    SDL_RenderCopy(render, splashscreen, NULL, NULL);

    SDL_Rect lmb_rect, omb_rect, pvc_rect;
    SDL_QueryTexture(local_multi_btn, NULL, NULL, &lmb_rect.w, &lmb_rect.h);
    lmb_rect.x = windowWidth/2 - lmb_rect.w/2;
    lmb_rect.y = windowHeight/2 + lmb_rect.h - 10;
    SDL_QueryTexture(online_multi_btn, NULL, NULL, &omb_rect.w, &omb_rect.h);
    omb_rect.x = windowWidth/2 - omb_rect.w/2;
    omb_rect.y = lmb_rect.y + lmb_rect.h + 20;
    SDL_QueryTexture(playervcpu_btn, NULL, NULL, &pvc_rect.w, &pvc_rect.h);
    pvc_rect.x = windowWidth/2 - pvc_rect.w/2;
    pvc_rect.y = omb_rect.y + omb_rect.h + 20;

    SDL_RenderCopy(render, local_multi_btn, NULL, &lmb_rect);
    SDL_RenderCopy(render, online_multi_btn, NULL, &omb_rect);
    SDL_RenderCopy(render, playervcpu_btn, NULL, &pvc_rect);

    SDL_RenderPresent(render);

    // wait for the player to chose the gamemode
    Gamemode mode = CHOOSING;
    SDL_Event event;
    while (mode == CHOOSING) {
        if (SDL_WaitEvent(&event)) {
            if (event.type == SDL_QUIT)
                mode = QUIT;
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x = event.button.x;
                int y = event.button.y;
                if (point_in_rect(lmb_rect, x, y)) {
                    mode = LOCAL_MULTI;
                } else if (point_in_rect(omb_rect, x, y)) {
                    mode = ONLINE_MULTI;
                } else if (point_in_rect(pvc_rect, x, y)) {
                    mode = PvC;
                }
            }

        }
    }

    return mode;
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
void local_multiplayer(SDL_Window* window, SDL_Renderer* render, char* map_filename) {
    Map map;
    if (read_map_from_file(&map, map_filename) != 0)
    {
        printf("Error while opening the map file (%s)\n", SDL_GetError());
    }
    init_bonus(&map); // randomly add hidden bonuses in soft walls

    // Resize the window to fit the map
    SDL_SetWindowSize(window, map.size * TILE_SIZE, map.size * TILE_SIZE);

    // Init the players
    const short nPlayers = 2;
    Player players[nPlayers];
    Key controls1[5] = {K_z, K_d, K_s, K_q, K_SPACE};
    Key controls2[5] = {K_UP, K_RIGHT, K_DOWN, K_LEFT, K_RSHIFT};
    init_player(&players[0], 1, 1, controls1);
    init_player(&players[1], map.size-2, map.size-2, controls2);

    // Load and display map and players
    load_all_textures(render);
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
    TTF_Font* sans = TTF_OpenFont("../assets/nasa.ttf", 24);
    if (sans == NULL) {
        printf("Error while loading font: %s\n", TTF_GetError());
        return;
    }
    struct timespec start_time;
    clock_gettime(CLOCK_REALTIME, &start_time);
    SDL_Rect Message_rect; //create a rect
    int ww, wh;
    SDL_GetWindowSize(window, &ww, &wh);
    Message_rect.w = 100; // controls the width of the rect
    Message_rect.h = 100; // controls the height of the rect
    Message_rect.y = 10; // controls the rect's y coordinte
    Message_rect.x = ww / 2 - Message_rect.w / 2;  //controls the rect's x coordinate 

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
        for (int i = 0; i < nPlayers; i++)
            display_bombs(render, &map, &players[i]);
        for (int i = 0; i < nPlayers; i++) {
            if (players[i].isAlive) display_player(render, &players[i]);
        }
        SDL_RenderCopy(render, message, NULL, &Message_rect);
        SDL_RenderPresent(render);
    }
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

    // Start the game
    Gamemode gamemode = choose_gamemode(render, windowWidth, windowHeight);
    if (gamemode == LOCAL_MULTI)
        local_multiplayer(window, render, map_filename);

    exit_status = EXIT_SUCCESS;
Quit:
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return exit_status;
}