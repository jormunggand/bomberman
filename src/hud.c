#include "hud.h"

#include "utils.h"
#include "player.h"
#include "map.h"
#include <SDL_ttf.h>

SDL_Texture* splashscreen;
SDL_Texture* local_multi_btn;
SDL_Texture* online_multi_btn;
SDL_Texture* playervcpu_btn;
SDL_Texture* host_btn;
SDL_Texture* join_btn;
SDL_Texture* back_btn;

// load textures needed for the booting menu
int load_menu_textures(SDL_Renderer* render) {
    splashscreen = loadImage("../assets/title_flat.jpg", render);
    local_multi_btn = loadImage("../assets/Menu/local_multiplayer_button.png", render);
    online_multi_btn = loadImage("../assets/Menu/online_multiplayer_button.png", render);
    playervcpu_btn = loadImage("../assets/Menu/player_vs_cpu_button.png", render);

    host_btn = loadImage("../assets/Menu/host_button.png", render);
    join_btn = loadImage("../assets/Menu/join_button.png", render);

    back_btn = loadImage("../assets/Menu/back_button.png", render);

    if (splashscreen == NULL || local_multi_btn == NULL || online_multi_btn == NULL 
        || playervcpu_btn == NULL || host_btn == NULL || join_btn == NULL) {
        printf("%s\n", SDL_GetError());
        return -1;      
    }
    return 0;
}

void draw_button(SDL_Renderer* render, SDL_Texture* text, int x, int y) {
    SDL_Rect btn_rect;
    SDL_QueryTexture(text, NULL, NULL, &btn_rect.w, &btn_rect.h);
    btn_rect.x = x;
    btn_rect.y = y;

    SDL_RenderCopy(render, text, NULL, &btn_rect);
    SDL_RenderPresent(render);
}

// display the screen where the player can choose the gamemode
Gamemode choose_gamemode(SDL_Renderer* render, int windowWidth, int windowHeight) {
    // display the background and the buttons
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
                    mode = ONLINE_MENU;
                } else if (point_in_rect(pvc_rect, x, y)) {
                    mode = PvC;
                }
            }
        }
    }

    return mode;
}

Gamemode online_menu(SDL_Renderer* render, int windowWidth, int windowHeight) {
    // Blue background
    SDL_SetRenderDrawColor(render, 37, 150, 190, 255);
    SDL_RenderClear(render);

    // Display buttons
    SDL_Rect host_rect, join_rect, back_rect;
    SDL_QueryTexture(host_btn, NULL, NULL, &host_rect.w, &host_rect.h);
    SDL_QueryTexture(join_btn, NULL, NULL, &join_rect.w, &join_rect.h);
    SDL_QueryTexture(back_btn, NULL, NULL, &back_rect.w, &back_rect.h);
    host_rect.x = windowWidth/2 - host_rect.w/2;
    host_rect.y = windowHeight/2 - host_rect.h/2;
    join_rect.x = windowWidth/2 - join_rect.w/2;
    join_rect.y = host_rect.y + join_rect.h + 20;
    back_rect.x = windowWidth/2 - back_rect.w/2;
    back_rect.y  = join_rect.y + back_rect.h + 20;
    SDL_RenderCopy(render, host_btn, NULL, &host_rect);
    SDL_RenderCopy(render, join_btn, NULL, &join_rect);
    SDL_RenderCopy(render, back_btn, NULL, &back_rect);

    SDL_RenderPresent(render);

    // Let the user choose
    SDL_Event event;
    while (event.type != SDL_QUIT) {
        if (SDL_WaitEvent(&event) && event.type == SDL_MOUSEBUTTONDOWN) {
            int x = event.button.x;
            int y = event.button.y;
            if (point_in_rect(back_rect, x, y)) {
                return CHOOSING;
            } else if (point_in_rect(host_rect, x, y)) {
                return ONLINE_HOSTING;
            } else if (point_in_rect(join_rect, x, y)) {
                return ONLINE_JOINING;
            }
        }
    }
    return QUIT;
}

void display_hud(SDL_Renderer* render, TTF_Font* sans, Player* p1, Player* p2, int windowWidth){
    int icon_size = 3 * HUD_HEIGHT / 4;
    SDL_Color white = {255, 255, 255};

    SDL_Rect bomb_p1 = {0, 0, icon_size, icon_size};    
    SDL_Rect flame_p1 = {windowWidth / 7, 0, icon_size, icon_size};
    SDL_Rect speed_p1 = {2 * windowWidth / 7, 0, icon_size, icon_size};
    SDL_Rect bomb_p2 = {4 * windowWidth / 7, 0, icon_size, icon_size};
    SDL_Rect flame_p2 = {5 * windowWidth / 7, 0, icon_size, icon_size};
    SDL_Rect speed_p2 = {6 * windowWidth / 7, 0, icon_size, icon_size};

    SDL_Rect bomb_p1_text_rect = {icon_size + 5, 0, icon_size, icon_size};
    SDL_Rect flame_p1_text_rect = {windowWidth / 7 + icon_size + 5, 0, 2 * icon_size, icon_size};
    SDL_Rect speed_p1_text_rect = {2 * windowWidth / 7 + icon_size + 5, 0, 2 * icon_size, icon_size};
    SDL_Rect bomb_p2_text_rect = {4 * windowWidth / 7 + icon_size + 5, 0, icon_size, icon_size};
    SDL_Rect flame_p2_text_rect = {5 * windowWidth / 7 + icon_size + 5, 0, 2 * icon_size, icon_size};
    SDL_Rect speed_p2_text_rect = {6 * windowWidth / 7 + icon_size + 5, 0, 2 * icon_size, icon_size};

    char bomb_p1_str[5];
    char flame_p1_str[5];
    char speed_p1_str[10];
    char bomb_p2_str[5];
    char flame_p2_str[5];
    char speed_p2_str[10];

    sprintf(bomb_p1_str, ": %d", p1->nMaxBombs);
    sprintf(flame_p1_str, ": %d", p1->flamePower);
    sprintf(speed_p1_str, ": %.2f", p1->speed);
    sprintf(bomb_p2_str, ": %d", p2->nMaxBombs);
    sprintf(flame_p2_str, ": %d", p2->flamePower);
    sprintf(speed_p2_str, ": %.2f", p2->speed);

    SDL_Surface* bomb_p1_surf = TTF_RenderText_Solid(sans, bomb_p1_str, white);
    SDL_Surface* flame_p1_surf = TTF_RenderText_Solid(sans, flame_p1_str, white);
    SDL_Surface* speed_p1_surf = TTF_RenderText_Solid(sans, speed_p1_str, white);
    SDL_Surface* bomb_p2_surf = TTF_RenderText_Solid(sans, bomb_p2_str, white);
    SDL_Surface* flame_p2_surf = TTF_RenderText_Solid(sans, flame_p2_str, white);
    SDL_Surface* speed_p2_surf = TTF_RenderText_Solid(sans, speed_p2_str, white);

    SDL_Texture* bomb_p1_text_texture = SDL_CreateTextureFromSurface(render, bomb_p1_surf);
    SDL_Texture* flame_p1_text_texture = SDL_CreateTextureFromSurface(render, flame_p1_surf);
    SDL_Texture* speed_p1_text_texture = SDL_CreateTextureFromSurface(render, speed_p1_surf);
    SDL_Texture* bomb_p2_text_texture = SDL_CreateTextureFromSurface(render, bomb_p2_surf);
    SDL_Texture* flame_p2_text_texture = SDL_CreateTextureFromSurface(render, flame_p2_surf);
    SDL_Texture* speed_p2_text_texture = SDL_CreateTextureFromSurface(render, speed_p2_surf);

    SDL_RenderCopy(render, bomb_bonus_icon, NULL, &bomb_p1);
    SDL_RenderCopy(render, flame_bonus_icon, NULL, &flame_p1);
    SDL_RenderCopy(render, speed_bonus_icon, NULL, &speed_p1);
    SDL_RenderCopy(render, bomb_bonus_icon, NULL, &bomb_p2);
    SDL_RenderCopy(render, flame_bonus_icon, NULL, &flame_p2);
    SDL_RenderCopy(render, speed_bonus_icon, NULL, &speed_p2);

    SDL_RenderCopy(render, bomb_p1_text_texture, NULL, &bomb_p1_text_rect);
    SDL_RenderCopy(render, flame_p1_text_texture, NULL, &flame_p1_text_rect);
    SDL_RenderCopy(render, speed_p1_text_texture, NULL, &speed_p1_text_rect);
    SDL_RenderCopy(render, bomb_p2_text_texture, NULL, &bomb_p2_text_rect);
    SDL_RenderCopy(render, flame_p2_text_texture, NULL, &flame_p2_text_rect);
    SDL_RenderCopy(render, speed_p2_text_texture, NULL, &speed_p2_text_rect);
}