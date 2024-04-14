#include <stdbool.h>
#ifdef __linux__
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#elif __APPLE__
#include <SDL.h>
#include <SDL_image.h>
#endif

void edge_collision(SDL_Window* window, SDL_Rect* rect, int** map, int vx, int vy);

bool check_collision(SDL_Rect* r, int** map);