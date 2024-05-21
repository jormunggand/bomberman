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

/* This file contains functions handling online multiplayer */
/* Does not work entirely yet (bombs are not displayed on the client side)*/

struct Map;
struct Player;


// Server side functions
int send_map(TCPsocket clientSocket, struct Map map);
int send_player_infos(TCPsocket clientSocket, struct Player player1, struct Player player2);
int online_multiplayer(SDL_Window *window, SDL_Renderer *render, char *map_filename, TCPsocket clientSocket);
int receiveControls(void *data);
int host_server(SDL_Window* window, SDL_Renderer* render, char* map_filename);


// Client side functions
int sendControls(void* data);
void get_map_first(TCPsocket serverSocket, struct Map* map);
void get_map(TCPsocket serverSocket, struct Map* map);
int get_player_infos(TCPsocket serverSocket, struct Player* player1, struct Player* player2);
int join_server(SDL_Window* window, SDL_Renderer *render);