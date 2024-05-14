#ifdef __linux__
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_ttf.h>
#elif __APPLE__
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_net.h>
#include <SDL_ttf.h>
#endif

int receiveMessages(void *data);
void host_server(SDL_Renderer* render, int windowWidth, int windowHeight);
void join_server(SDL_Renderer* render, int windowWidth, int windowHeight);