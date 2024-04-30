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

SDL_Texture* splashscreen;
SDL_Texture* local_multi_btn;
SDL_Texture* online_multi_btn;
SDL_Texture* playervcpu_btn;

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

void display_splashcreen(SDL_Renderer* render) {
    if (load_menu_textures(render) != 0)
        printf("%s\n", SDL_GetError());
    SDL_RenderCopy(render, splashscreen, NULL, NULL);

    SDL_Rect lmb_rect = {.x=300, .y=440, .w=0, .h=0};
    SDL_QueryTexture(local_multi_btn, NULL, NULL, &lmb_rect.w, &lmb_rect.h);
    SDL_RenderCopy(render, local_multi_btn, NULL, &lmb_rect);

    SDL_RenderPresent(render);
    SDL_Delay(5000);
    return;
}

void play_game(SDL_Window* window, SDL_Renderer* render, char* map_filename) {
    Map map;
    if (read_map_from_file(&map, map_filename) != 0) {
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

    double now = (double) clock(); 
    double last = 0.0;
    double deltaTime;
    double targetfps = 1.0 / 60.0;
    double accumulator = 0.0;

    // int cpt = 0;

    while (!done) {
        last = now;
        now = (double) clock();
        deltaTime = (now - last) / (double) CLOCKS_PER_SEC; // delta time in seconds
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
    display_splashcreen(render);
    play_game(window, render, map_filename);


    exit_status = EXIT_SUCCESS;
Quit:
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return exit_status;
}