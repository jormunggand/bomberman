#ifdef __linux__
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#elif __APPLE__
#include <SDL.h>
#include <SDL_image.h>
#endif


struct Player;

typedef enum {
    LOCAL_MULTI,
    ONLINE_MENU, 
    ONLINE_HOSTING,
    ONLINE_JOINING,
    PvC,
    CHOOSING, // the player has not chosed yet
    QUIT  // the player has closed the window
} Gamemode;

int load_menu_textures(SDL_Renderer* render);
void draw_button(SDL_Renderer* render, SDL_Texture* text, int baseX, int baseY);
Gamemode choose_gamemode(SDL_Renderer* render, int windowWidth, int windowHeight);
Gamemode online_menu(SDL_Renderer* render, int windowWidth, int windowHeight);
void display_hud(SDL_Renderer* render, TTF_Font* sans, struct Player* p1, struct Player* p2, int windowWidth);
int display_game_over(SDL_Renderer* render, TTF_Font* sans, struct Player* players, int ww, int wh);