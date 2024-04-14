#ifdef __linux__
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#elif __APPLE__
#include <SDL.h>
#include <SDL_image.h>
#endif

void loadTextures(SDL_Renderer* render);
int** read_map_from_file(char* file_name, int map_width, int map_height);
void display_map(SDL_Renderer* renderer, int** map, int map_width, int map_height);
void destroy_map(int** map, int map_width, int map_height);