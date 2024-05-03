#ifdef __linux__
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#elif __APPLE__
#include <SDL.h>
#include <SDL_image.h>
#endif

#include <stdbool.h>
#include <time.h>


#define NB_BOMB_TEXTURES (7)
#define NB_FLAME_TEXTURES (5)

struct Player;
struct Map;
struct Tile;

typedef struct Bomb {
    SDL_Rect rect;
    struct timespec start_time;
    int radius; // does not count the center tile
    bool detonated;
    bool* explosion_tiles; // array of size (2 * radius + 1)^2 to represent on which tiles the explosion must be displayed 
    struct Player* owner; // the player who placed the bomb
} Bomb;

int ij_to_expl_index(int i, int j, int radius);
void exlp_index_to_ij(int expl_index, int radius, int* i, int* j);

void init_bomb(Bomb* bomb, int x, int y, struct Player* player);
void player_place_bomb(struct Player* player, struct Map* map);
void display_explosion(SDL_Renderer* render, SDL_Texture* texture, Bomb* bomb, struct Map* map, struct Player* player);
int display_bomb(SDL_Renderer* render, struct Tile* tile, struct Map* map, struct Player* player);
void display_bombs(SDL_Renderer* render, struct Map* map, struct Player* player);
int load_bomb_aux(SDL_Renderer* render, char* base, SDL_Texture** textures, int nb_textures);
int load_bomb_textures(SDL_Renderer* render);