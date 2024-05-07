#include "hud.h"

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