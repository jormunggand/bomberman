#ifdef __linux__
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#elif __APPLE__
#include <SDL.h>
#include <SDL_image.h>
#endif

#include <stdbool.h>

#define ANIMATION_FRAMES (8)

typedef enum {
    FRONT,
    BACK,
    RIGHT,
    LEFT
} SpriteDirection;


typedef struct {
    SpriteDirection curDir;
    int iframe;
    SDL_Texture** animations;
    SDL_Rect rect;
} Player;

void init_player(Player* player, int x, int y);
void change_direction(Player* player, SpriteDirection newDir);
void update_sprite(Player* player);
void display_player(SDL_Renderer* render, Player player);
void destroy_player(Player* player);

int load_animations_aux(SDL_Renderer* render, char* base, SDL_Texture** textures);
int load_animations(SDL_Renderer* render);

void edge_collision(SDL_Window* window, SDL_Rect* player_rect, SDL_Rect* collision_rect, int** map, int velx, int vely);
bool check_collision(SDL_Rect* r, int** map);