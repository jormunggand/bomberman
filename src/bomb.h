#ifdef __linux__
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#elif __APPLE__
#include <SDL.h>
#include <SDL_image.h>
#endif

#include <stdbool.h>

#define NB_BOMB_TEXTURES (3)
#define NB_FLAME_TEXTURES (5)

struct Player;
struct Map;

typedef struct Bomb {
    SDL_Rect rect;
    int nb_ticks;
    int radius; // does not count the center tile
    bool detonated;
    bool* explosion_tiles; // array of size (2 * radius + 1)^2 to represent on which tiles the explosion must be displayed 
} Bomb;

int ij_to_expl_index(int i, int j, int radius);
void exlp_index_to_ij(int expl_index, int radius, int* i, int* j);

void init_bomb(Bomb* bomb, int x, int y, int radius);
void add_bomb(struct Map* map, int x, int y, int radius);
void player_place_bomb(struct Player* player, struct Map* map);
void display_explosion(SDL_Renderer* render, SDL_Texture* texture, Bomb* bomb, struct Map* map);
int display_bomb(SDL_Renderer* render, Bomb* bomb, struct Map* map);
void display_bombs(SDL_Renderer* render, struct Map* map);
int load_bomb_aux(SDL_Renderer* render, char* base, SDL_Texture** textures, int nb_textures);
int load_bomb_textures(SDL_Renderer* render);