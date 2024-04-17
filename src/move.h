#include <stdbool.h>
#ifdef __linux__
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#elif __APPLE__
#include <SDL.h>
#include <SDL_image.h>
#endif

#include <stdbool.h>

#define ANIMATION_FRAMES (8)

// Powerups constnts
#define BASE_SPEED (16) // in pixels
#define SPEED_BOOST (4) // in pixels
#define MAX_SPEED (28)

#define MAX_BOMBS (5)
#define MAX_POWER (5) // in number of tiles

struct Map;

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
    int speed;
    int flamePower;
} Player;

void init_player(Player* player, int x, int y);
void change_direction(Player* player, SpriteDirection newDir);
void update_sprite(Player* player);
void display_player(SDL_Renderer* render, Player* player);
//void destroy_player(Player* player);

int load_animations_aux(SDL_Renderer* render, char* base, SDL_Texture** textures);
int load_animations(SDL_Renderer* render);

void edge_collision(SDL_Window* window, Player* player, struct Map *map, int velx, int vely);
bool check_collision(SDL_Rect* r, struct Map *map);
void get_bonus(Player* player, struct Map* map);
