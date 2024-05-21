#ifdef __linux__
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#elif __APPLE__
#include <SDL.h>
#include <SDL_image.h>
#endif

#include <stdbool.h>
#include <time.h>

/* This file contains the code related to handling bombs and their explosions */



#define NB_BOMB_TEXTURES (7)
#define NB_FLAME_TEXTURES (5)
#define MAX_BOMBS (20)

// this array will hold every bomb currently active on the screen
// bombs[i] is either NULL or has a struct Bomb pointer
extern struct Bomb* bombs[MAX_BOMBS];

struct Player;
struct Map;
struct Tile;

typedef struct Bomb {
    SDL_Rect rect;
    SDL_Rect collision_rect;
    struct timespec start_time;
    int radius; // does not count the center tile
    bool detonated; // true if the bomb has exploded
    bool* explosion_tiles; // array of size (4 * radius + 1) to represent on which tiles the explosion must be displayed 
    struct Player* owner; // the player who placed the bomb
    bool isMoving; // true if the bomb is moving
    int direction; // the direction the bomb is moving
} Bomb;

// useful indexes conversion
int ij_to_expl_index(int i, int j, int radius);
void exlp_index_to_ij(int expl_index, int radius, int* i, int* j);

// initializes a bomb at the given position in map coordinates owned by the given player
void init_bomb(Bomb* bomb, int x, int y, struct Player* player);

// makes the player place a bomb at its position if possible
void player_place_bomb(struct Player* player, struct Map* map);

// displays the explosion of the bomb
void display_explosion(SDL_Renderer* render, SDL_Texture* texture, Bomb* bomb, struct Map* map);

// updates the postions of the moving bombs
void update_bombs_positions(SDL_Window* window, struct Map* map, double dt);

// updates the death status of the player
void updateDeathStatus(struct Map* map, struct Player* player);

// displays the bomb or the flames depending on the bomb's timer
int display_bomb(SDL_Renderer* render, Bomb* bomb, struct Map* map);

// displays all the bombs present on the map
void display_bombs(SDL_Renderer* render, struct Map* map);

// auxiliar function to load the bomb textures
int load_bomb_aux(SDL_Renderer* render, char* base, SDL_Texture** textures, int nb_textures);

// loads the bomb textures
int load_bomb_textures(SDL_Renderer* render);

// frees the bomb objects
void free_bombs();

/* checks if the bomb rect is colliding with another bomb and update which bomb is moving
the sign parameter is usid to determine where the incoming bomb is coming from */
bool bombToBombCollision(SDL_Rect* r, int sign, int bombIndex);