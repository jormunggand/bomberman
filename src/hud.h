#ifdef __linux__
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#elif __APPLE__
#include <SDL.h>
#include <SDL_image.h>
#endif

typedef enum {
    LOCAL_MULTI,
    ONLINE_MULTI,
    PvC,
    CHOOSING, // the player has not chosed yet
    QUIT  // the player has closed the window
} Gamemode;

int load_menu_textures(SDL_Renderer* render);
Gamemode choose_gamemode(SDL_Renderer* render, int windowWidth, int windowHeight);