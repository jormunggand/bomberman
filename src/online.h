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

struct Map;

int online_multiplayer(SDL_Window *window, SDL_Renderer *render, char *map_filename, TCPsocket clientSocket);
//int send_map(TCPsocket clientSocket, Map map);
int receiveControls(void *data);
int host_server(SDL_Window* window, SDL_Renderer* render, char* map_filename);

//void decode_map(Map* map, char* encodedMap);
int sendControls(void* data);
int join_server(SDL_Window* window, SDL_Renderer *render);