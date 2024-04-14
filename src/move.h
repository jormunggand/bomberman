#ifdef __linux__
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#elif __APPLE__
#include <SDL.h>
#include <SDL_image.h>
#endif

#define ANIMATION_FRAMES (8)

typedef enum {
    FRONT,
    BACK,
    RIGHT,
    LEFT
} SpriteDirection;


typedef struct  {
    SpriteDirection curDir;
    int iframe;
    SDL_Texture** animations;
    SDL_Rect* rect;
} Player;

Player* create_player(int x, int y);
void change_direction(Player* player, SpriteDirection newDir);
void display_player(SDL_Renderer* render, Player* player);
void destroy_player(Player* player);

int load_animations_aux(SDL_Renderer* render, char* base, SDL_Texture** textures);
int load_animations(SDL_Renderer* render);

void edge_collision(SDL_Window* window, SDL_Rect* rect, int** map);