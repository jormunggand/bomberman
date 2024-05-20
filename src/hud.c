#include "hud.h"

#include "utils.h"
#include "player.h"
#include "map.h"
#include "bomb.h"
#include <SDL_ttf.h>

SDL_Texture* splashscreen;
SDL_Texture* local_multi_btn;
SDL_Texture* online_multi_btn;
SDL_Texture* playervcpu_btn;
SDL_Texture* host_btn;
SDL_Texture* join_btn;
SDL_Texture* back_btn;

SDL_Texture* bomb_p1_text_texture = NULL;
SDL_Texture* flame_p1_text_texture = NULL;
SDL_Texture* speed_p1_text_texture = NULL;
SDL_Texture* bomb_p2_text_texture = NULL;
SDL_Texture* flame_p2_text_texture = NULL;
SDL_Texture* speed_p2_text_texture = NULL;

// load textures needed for the booting menu
int load_menu_textures(SDL_Renderer* render) {
    splashscreen = loadImage("../assets/title_flat.jpg", render, false);
    local_multi_btn = loadImage("../assets/Menu/local_multiplayer_button.png", render, false);
    online_multi_btn = loadImage("../assets/Menu/online_multiplayer_button.png", render, false);
    playervcpu_btn = loadImage("../assets/Menu/player_vs_cpu_button.png", render, false);

    host_btn = loadImage("../assets/Menu/host_button.png", render, false);
    join_btn = loadImage("../assets/Menu/join_button.png", render, false);

    back_btn = loadImage("../assets/Menu/back_button.png", render, false);

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

// display the HUD with the bonus icons and the number of bombs, flame power and speed
void display_hud(SDL_Renderer* render, TTF_Font* sans, Player* p1, Player* p2, int windowWidth){
    int icon_size = 5 * HUD_HEIGHT / 8;
    int y = HUD_HEIGHT / 2 - icon_size / 2;
    int xleft = 10;
    SDL_Color white = {255, 255, 255};

    SDL_Rect bomb_p1 = {xleft, y, icon_size, icon_size};    
    SDL_Rect flame_p1 = {xleft + windowWidth / 7, y, icon_size, icon_size};
    SDL_Rect speed_p1 = {xleft + 2 * windowWidth / 7, y, icon_size, icon_size};
    SDL_Rect bomb_p2 = {4 * windowWidth / 7, y, icon_size, icon_size};
    SDL_Rect flame_p2 = {5 * windowWidth / 7, y, icon_size, icon_size};
    SDL_Rect speed_p2 = {6 * windowWidth / 7, y, icon_size, icon_size};

    SDL_Rect bomb_p1_text_rect = {xleft + icon_size + 5, y, icon_size, icon_size};
    SDL_Rect flame_p1_text_rect = {xleft + windowWidth / 7 + icon_size + 5, y, icon_size, icon_size};
    SDL_Rect speed_p1_text_rect = {xleft + 2 * windowWidth / 7 + icon_size + 5, y, 2 * icon_size, icon_size};
    SDL_Rect bomb_p2_text_rect = {4 * windowWidth / 7 + icon_size + 5, y, icon_size, icon_size};
    SDL_Rect flame_p2_text_rect = {5 * windowWidth / 7 + icon_size + 5, y, icon_size, icon_size};
    SDL_Rect speed_p2_text_rect = {6 * windowWidth / 7 + icon_size + 5, y, 2 * icon_size, icon_size};

    char bomb_p1_str[5];
    char flame_p1_str[5];
    char speed_p1_str[10];
    char bomb_p2_str[5];
    char flame_p2_str[5];
    char speed_p2_str[10];

    sprintf(bomb_p1_str, ": %d", p1->nMaxBombs - p1->nCurBombs);
    sprintf(flame_p1_str, ": %d", p1->flamePower);
    sprintf(speed_p1_str, ": x%.2f", p1->speed  / BASE_SPEED);
    sprintf(bomb_p2_str, ": %d", p2->nMaxBombs - p2->nCurBombs);
    sprintf(flame_p2_str, ": %d", p2->flamePower);
    sprintf(speed_p2_str, ": x%.2f", p2->speed / BASE_SPEED);

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

    SDL_FreeSurface(bomb_p1_surf);
    SDL_FreeSurface(flame_p1_surf);
    SDL_FreeSurface(speed_p1_surf);
    SDL_FreeSurface(bomb_p2_surf);
    SDL_FreeSurface(flame_p2_surf);
    SDL_FreeSurface(speed_p2_surf);

    SDL_DestroyTexture(bomb_p1_text_texture);
    SDL_DestroyTexture(flame_p1_text_texture);
    SDL_DestroyTexture(speed_p1_text_texture);
    SDL_DestroyTexture(bomb_p2_text_texture);
    SDL_DestroyTexture(flame_p2_text_texture);
    SDL_DestroyTexture(speed_p2_text_texture);
}


int display_game_over(SDL_Renderer* render, TTF_Font* sans, Player* players, int ww, int wh) {
    SDL_Color Black = {0, 0, 0};
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
    bool done = false;
    int res = -1;
    while (!done) {
        if (SDL_WaitEvent(&event)) {
            if (event.type == SDL_QUIT) {
                res = QUIT;
                done = true;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x = event.button.x;
                int y = event.button.y;
                if (point_in_rect(yes_rect, x, y)) {
                    res = LOCAL_MULTI;
                    done = true;
                } else if (point_in_rect(no_rect, x, y)) {
                    res = CHOOSING;
                    done = true;
                }
            }
        }
    }
    // destroy textures and surfaces
    SDL_FreeSurface(surface_go);
    SDL_FreeSurface(surface_choice);
    SDL_FreeSurface(surface_yes);
    SDL_FreeSurface(surface_no);

    SDL_DestroyTexture(mess_go);
    SDL_DestroyTexture(mess_choice);
    SDL_DestroyTexture(mess_yes);
    SDL_DestroyTexture(mess_no);

    return res;
}