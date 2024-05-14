#ifdef __linux__
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_net.h>
#elif __APPLE__
#include <SDL.h>
#include <SDL_image.h>
#inlcude <SDL_net.h>
#endif

void host_server(SDL_Renderer* render, int windowWidth, int windowHeight);
void join_server(SDL_Renderer* render, int windowWidth, int windowHeight);