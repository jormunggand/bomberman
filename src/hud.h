#ifdef __linux__
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#elif __APPLE__
#include <SDL.h>
#include <SDL_image.h>
#endif

#include <SDL_ttf.h>

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


// load the button textures used in the menu and return -1 if an error occured or 0 otherwise
int load_menu_textures(SDL_Renderer* render);

// draw the button at the given position
void draw_button(SDL_Renderer* render, SDL_Texture* text, int baseX, int baseY);

// display the main menu and return the gamemode chosen by the player by clicking on the buttons
Gamemode choose_gamemode(SDL_Renderer* render, int windowWidth, int windowHeight);

// display the menu inside the online multiplayer mode and return whether the player wants to host or join a game
Gamemode online_menu(SDL_Renderer* render, int windowWidth, int windowHeight);

// display the hud on top of the screen during the game
void display_hud(SDL_Renderer* render, TTF_Font* sans, struct Player* p1, struct Player* p2, int windowWidth);

// display the game over screen and return the gamemode chosen by the player after the game is over
int display_game_over(SDL_Renderer* render, TTF_Font* sans, struct Player* players, int ww, int wh);