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


typedef struct Player {
    SpriteDirection curDir;
    bool isWalking;
    int iframe;
    SDL_Texture** animations;
    SDL_Rect rect;
    SDL_Rect collisionRect;

    int nBombs;
    float speed;
    int flamePower;
} Player;

struct Map;

void init_player(Player* player, int x, int y);
void change_direction(Player* player, SpriteDirection newDir);
void update_sprite(Player* player);
void display_player(SDL_Renderer* render, Player* player);
//void destroy_player(Player* player);

int load_animations_aux(SDL_Renderer* render, char* base, SDL_Texture** textures);
int load_animations(SDL_Renderer* render);

void edge_collision(SDL_Window* window, Player* player, struct Map *map, float velx, float vely);
bool check_collision(SDL_Rect* r, struct Map *map);

