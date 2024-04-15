#include <stdbool.h>
#ifdef __linux__
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#elif __APPLE__
#include <SDL.h>
#include <SDL_image.h>
#endif

void edge_collision(SDL_Window* window, SDL_Rect* player_rect, SDL_Rect* collision_rect, int** map, int velx, int vely);

bool check_collision(SDL_Rect* r, int** map);