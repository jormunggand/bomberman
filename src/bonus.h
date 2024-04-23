#ifdef __linux__
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#elif __APPLE__
#include <SDL.h>
#include <SDL_image.h>
#endif



struct Player;
struct Map;

void init_bonus(struct Map* map);
void get_bonus(struct Player* player, struct Map* map);