#ifdef __linux__
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#elif __APPLE__
#include <SDL.h>
#include <SDL_image.h>
#endif



struct Player;
struct Map;

// randomly initializes the bonus on the map inside the soft walls given a given spawn rate
void init_bonus(struct Map* map);

// gives a bonus to the player if he touches it
void get_bonus(struct Player* player, struct Map* map);