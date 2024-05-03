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

    int nMaxBombs; // the maximum number of bombs the player can fire simultaneously
    int nCurBombs; // the current number of bombs the player has lit
    float speed;
    int flamePower;

    bool isAlive;
} Player;

struct Map;

void init_player(Player* player, int x, int y);
void change_direction(Player* player, SpriteDirection newDir);
void update_sprite(Player* player);
void display_player(SDL_Renderer* render, Player* player);

int load_player_aux(SDL_Renderer* render, char* base, SDL_Texture** textures);
int load_player_textures(SDL_Renderer* render);

void edge_collision(SDL_Window* window, Player* player, struct Map *map, int deltaX, int deltaY, double elapsedTime);
bool check_collision(SDL_Rect* r, struct Map *map);
bool bomb_collision(SDL_Rect* r, struct Map *map);

