#include "online.h"
#include "map.h"
#include "player.h"
#include "utils.h"
#include "keyboard.h"
#include "bomb.h"
#include "bonus.h"
#include "hud.h"

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_MESSAGE_LENGTH (1024)
#define BILLION (1000000000L)

SDL_KeyboardEvent recInput;
// to store player 2's input received through the network

int send_map(TCPsocket clientSocket, Map map)
{
    char encodedMap[MAX_MESSAGE_LENGTH];
    char encodedBonus[MAX_MESSAGE_LENGTH];
    sprintf(encodedMap, "%d", map.size);
    int k = 2;
    for (int i = 0; i < map.size; i++)
        for (int j = 0; j < map.size; j++) {
            encodedBonus[k-2] = (map.grid[i][j].bonus + '0');
            encodedMap[k++] = (map.grid[i][j].type + '0');
        }

    //printf("Sending %s", encodedMap);
    int size = strlen(encodedMap);
    encodedMap[size] = '\0';
    int bytesSent = SDLNet_TCP_Send(clientSocket, encodedMap, 1024);
    if (bytesSent < size)
    {
        printf("Error while sending controls (only %d bytes were sent)\n", bytesSent);
        printf("%s\n", SDLNet_GetError());
        return -1;
    }

    size = strlen(encodedBonus);
    encodedBonus[size] = '\0';
    bytesSent = SDLNet_TCP_Send(clientSocket, encodedBonus, 1024);
    if (bytesSent < size)
    {
        printf("Error while sending controls (only %d bytes were sent)\n", bytesSent);
        printf("%s\n", SDLNet_GetError());
        return -1;
    }


    return 0;
}

int send_player_infos(TCPsocket clientSocket, Player player1, Player player2) {
    char infos[MAX_MESSAGE_LENGTH];
    sprintf(infos, "%d %d %d %d %d %d %d %d %d %d %d %d", player1.iframe, player1.isWalking, player1.curDir, 
    player1.cpt_reset, player1.rect.x, player1.rect.y, player2.iframe, player2.isWalking, player2.curDir, 
    player2.cpt_reset, player2.rect.x, player2.rect.y);

    int size = strlen(infos);
    infos[size] = '\0';
    int bytesSent = SDLNet_TCP_Send(clientSocket, infos, 1024);
    if (bytesSent < size)
    {
        printf("Error while sending controls (only %d bytes were sent)\n", bytesSent);
        printf("%s\n", SDLNet_GetError());
        return -1;
    }
    return 0;
}

int online_multiplayer(SDL_Window *window, SDL_Renderer *render, char *map_filename, TCPsocket clientSocket)
{
    Map map;
    if (read_map_from_file(&map, map_filename) != 0)
    {
        printf("Error while opening the map file (%s)\n", SDL_GetError());
    }
    init_bonus(&map); // randomly add hidden bonuses in soft walls

    send_map(clientSocket, map); // send the map to the client


    // Resize the window to fit the map
    SDL_SetWindowSize(window, map.size * TILE_SIZE, map.size * TILE_SIZE);

    // Init the players
    const short nPlayers = 2;
    Player players[nPlayers];
    Key controls1[5] = {K_z, K_d, K_s, K_q, K_SPACE};
    Key controls2[5] = {K_UP, K_RIGHT, K_DOWN, K_LEFT, K_RSHIFT};
    init_player(&players[0], 1, 1, controls1, 0);
    init_player(&players[1], map.size-2, map.size-2, controls2, 1);

    // Load and display map and players
    display_map(render, &map);
    for (int i = 0; i < nPlayers; i++)
        display_player(render, &players[i]);
    SDL_RenderPresent(render);

    SDL_Event event;
    KeyboardHandler handler; // to handle simultaneous keypresses
    initHandler(&handler);

    bool done = false;
    Delta deltas[nPlayers];
    for (int i = 0; i < nPlayers; i++)
    {
        deltas[i].x = 0;
        deltas[i].y = 0;
    }

    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;
    double deltaTime = 0;
    double targetfps = 1.0 / 60.0;
    double accumulator = 0.0;

    char timerStr[8];
    SDL_Color White = {255, 255, 255};
    TTF_Font *sans = TTF_OpenFont("../assets/nasa.ttf", 12);
    if (sans == NULL)
    {
        printf("Error while loading font: %s\n", TTF_GetError());
        return QUIT;
    }
    struct timespec start_time;
    clock_gettime(CLOCK_REALTIME, &start_time);
    SDL_Rect Message_rect; // create a rect
    int ww, wh;
    SDL_GetWindowSize(window, &ww, &wh);
    Message_rect.w = 50;                          // controls the width of the rect
    Message_rect.h = 50;                          // controls the height of the rect
    Message_rect.y = 0;                           // controls the rect's y coordinte
    Message_rect.x = ww / 2 - Message_rect.w / 2; // controls the rect's x coordinate

    while (!done) 
    {
        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();
        deltaTime = (double)((NOW - LAST) / (double)SDL_GetPerformanceFrequency()); // delta time in seconds
        accumulator += deltaTime;

        int eventPresent = SDL_PollEvent(&event);
        if (eventPresent)
        {
            if (event.type == SDL_QUIT || handler.keyState[K_ESC] == SDL_PRESSED)
                done = true;
            else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
            {
                if (is_in_tab(sdl_to_k(event.key.keysym.sym), controls1, 5))
                {
                    handleEvent(&handler, event.key);
                }
            }
        }
        // printf("%d %d", recInput.keysym.sym, recInput.state);
        handleEvent(&handler, recInput); // player 2's input
        while (accumulator > targetfps)
        {
            for (int iPlayer = 0; iPlayer < nPlayers; iPlayer++)
            {
                if (!players[iPlayer].isAlive)
                    continue;
                Player *curPlayer = &players[iPlayer];
                if (anyDirectionPressed(&handler, iPlayer + 1))
                {
                    curPlayer->isWalking = true;
                    curPlayer->cpt_reset++;
                    update_sprite(curPlayer);
                    if (curPlayer->cpt_reset == 200)
                    {
                        curPlayer->cpt_reset = 0;
                        curPlayer->isWalking = false;
                    }

                    if (handler.keyState[curPlayer->controls[0]] == SDL_PRESSED)
                    {
                        deltas[iPlayer].y -= 1;
                        change_direction(curPlayer, BACK, iPlayer);
                    }
                    if (handler.keyState[curPlayer->controls[1]] == SDL_PRESSED)
                    {
                        deltas[iPlayer].x += 1;
                        change_direction(curPlayer, RIGHT, iPlayer);
                    }
                    if (handler.keyState[curPlayer->controls[2]] == SDL_PRESSED)
                    {
                        deltas[iPlayer].y += 1;
                        change_direction(curPlayer, FRONT, iPlayer);
                    }
                    if (handler.keyState[curPlayer->controls[3]] == SDL_PRESSED)
                    {
                        deltas[iPlayer].x -= 1;
                        change_direction(curPlayer, LEFT, iPlayer);
                    }
                }
                if (handler.keyState[curPlayer->controls[4]] == SDL_PRESSED)
                    player_place_bomb(curPlayer, &map);

                edge_collision(window, curPlayer, &map, deltas[iPlayer].x, deltas[iPlayer].y, targetfps);
                deltas[iPlayer].x = 0;
                deltas[iPlayer].y = 0;
                get_bonus(curPlayer, &map);
            }
            send_map(clientSocket, map);
            send_player_infos(clientSocket, players[0], players[1]);
            update_bombs_positions(window, &map, targetfps);
            accumulator -= targetfps;
        }
        struct timespec cur_time;
        clock_gettime(CLOCK_REALTIME, &cur_time);
        double dt = (cur_time.tv_sec - start_time.tv_sec) + (double)(cur_time.tv_nsec - start_time.tv_nsec) / (double)BILLION;
        sprintf(timerStr, "%.2f", dt);
        SDL_Surface *surfaceMessage = TTF_RenderText_Solid(sans, timerStr, White);
        SDL_Texture *message = SDL_CreateTextureFromSurface(render, surfaceMessage);

        SDL_RenderClear(render);
        display_map(render, &map);
        display_bombs(render, &map);
        for (int b = 0; b < MAX_BOMBS && bombs[b] != NULL; b++)
        {
            SDL_RenderDrawRect(render, &bombs[b]->collision_rect);
        }
        for (int i = 0; i < nPlayers; i++)
        {
            updateDeathStatus(&map, &players[i]);
            if (players[i].isAlive)
            {
                display_player(render, &players[i]);
            }
        }
        renderTexture(render, message, NULL, &Message_rect);
        SDL_RenderPresent(render);
    }
    return QUIT;
}

int receiveControls(void *data)
{
    TCPsocket socket = (TCPsocket)data; // Casting de data en TCPsocket
    char message[MAX_MESSAGE_LENGTH];   // Buffer pour stocker les messages reçus

    while (1)
    {
        if (SDLNet_TCP_Recv(socket, message, MAX_MESSAGE_LENGTH) > 0) // Recevoir un message du client
        {
            recInput.state = (int)(message[0] - '0');
            recInput.keysym.sym = atoi(message + 1);
            // printf("Received: %d %d\n", recInput.keysym.sym, recInput.state);
        }
    }
    return 0;
}

int host_server(SDL_Window *window, SDL_Renderer *render, char *map_filename)
{
    IPaddress ip; // server's ip address
    TCPsocket serverSocket, clientSocket = NULL;

    if (0 != SDLNet_ResolveHost(&ip, NULL, 1234)) // listen on port 1234
        goto NetworkError;
    serverSocket = SDLNet_TCP_Open(&ip);
    if (serverSocket == NULL)
        goto NetworkError;

    // Draw waiting screen
    SDL_SetRenderDrawColor(render, 150, 150, 190, 255);
    SDL_RenderClear(render);

    TTF_Font *nasa = TTF_OpenFont("../assets/Fonts/nasa.ttf", 24);
    if (nasa == NULL)
    {
        printf("%s\n", TTF_GetError());
        return QUIT;
    }
    SDL_Color red = {.r = 255, .g = 0, .b = 0, .a = 255};
    SDL_Surface *surf = TTF_RenderText_Solid(nasa, "En attente du joueur 2...", red);
    SDL_Texture *mess = SDL_CreateTextureFromSurface(render, surf);

    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
    SDL_Rect mess_rect = {.w = 200, .h = 50, .x = windowWidth / 2 - 100, .y = windowHeight / 2 - 50};

    renderTexture(render, mess, NULL, &mess_rect);
    SDL_RenderPresent(render);

    // Wait for a connection
    SDL_Event event;
    Uint64 last = 0, now;
    int delta;
    while (clientSocket == NULL)
    {
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
            return QUIT;

        now = SDL_GetPerformanceCounter();
        delta = now - last;
        if (delta > 250) // wait 250ms before attempting a new connection
        {
            clientSocket = SDLNet_TCP_Accept(serverSocket);
            last = now;
        }
    }
    printf("Client connected\n");

    SDL_Thread *recvThread = SDL_CreateThread(receiveControls, "ReceiveControls", (void *)clientSocket);
    // char message[MAX_MESSAGE_LENGTH]; // Buffer pour stocker les messages entrés par l'utilisateur

    int nextAction = online_multiplayer(window, render, map_filename, clientSocket);
    if (nextAction == QUIT)
        return QUIT;

    SDL_WaitThread(recvThread, NULL);

    SDLNet_TCP_Close(clientSocket);
    SDLNet_TCP_Close(serverSocket);
    return CHOOSING;

NetworkError:
    printf("Network Error: %s\n", SDLNet_GetError());
    return QUIT;
}

// send keyboard input to the server socket in data
// the format is a string "STATE*KEY_SCANCODE" where KEY_SCANCODE is the scancode of the key and STATE either
// SDL_PRESSED or SDL_RELEASED (1 or 0 respectively)
int sendControls(void *data)
{
    TCPsocket socket = (TCPsocket)data; // Casting de data en TCPsocket
    char message[MAX_MESSAGE_LENGTH];

    SDL_Event event;
    bool done = false;
    while (!done)
    {
        if (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
            {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    done = true;
                else
                {
                    sprintf(message, "%d%d", event.key.state, event.key.keysym.sym);
                    // printf("Sending %d %d\n", event.key.keysym.sym, event.key.state);
                    int size = strlen(message);
                    int bytesSent = SDLNet_TCP_Send(socket, message, size);
                    if (bytesSent < size)
                    {
                        printf("Error while sending controls (only %d bytes were sent)\n", bytesSent);
                        printf("%s\n", SDLNet_GetError());
                        return -1;
                    }
                }
            }
        }
    }
    return 0;
}


void get_map_first(TCPsocket serverSocket, Map* map) {
    char encodedMap[MAX_MESSAGE_LENGTH];
    char encodedBonus[MAX_MESSAGE_LENGTH];
    SDLNet_TCP_Recv(serverSocket, encodedMap, MAX_MESSAGE_LENGTH);
    SDLNet_TCP_Recv(serverSocket, encodedBonus, MAX_MESSAGE_LENGTH);

    char size_[3];
    size_[0] = encodedMap[0];
    size_[1] = encodedMap[1];
    size_[2] ='\0';

    map->size = atoi(size_);
    map->grid = malloc(map->size*sizeof(Tile*));
    int k = 2;
    for (int i = 0; i < map->size; i++) {
        map->grid[i] = malloc(map->size*sizeof(Tile));
        for (int j = 0; j < map->size; j++) {
            map->grid[i][j].bonus = encodedBonus[k-2] - '0';
            map->grid[i][j].type = encodedMap[k++] - '0';
        }
    }
}

void get_map(TCPsocket serverSocket, Map* map) 
{
    char encodedMap[MAX_MESSAGE_LENGTH];
    char encodedBonus[MAX_MESSAGE_LENGTH];
    SDLNet_TCP_Recv(serverSocket, encodedMap, MAX_MESSAGE_LENGTH);
    SDLNet_TCP_Recv(serverSocket, encodedBonus, MAX_MESSAGE_LENGTH);
    int k = 2;
    for (int i = 0; i < map->size; i++) {
        for (int j = 0; j < map->size; j++) {
            map->grid[i][j].bonus = encodedBonus[k-2] - '0';
            map->grid[i][j].type = encodedMap[k++] - '0';
        }
    }
}

int get_player_infos(TCPsocket serverSocket, Player* player1, Player* player2) {
    char infos[MAX_MESSAGE_LENGTH];
    SDLNet_TCP_Recv(serverSocket, infos, MAX_MESSAGE_LENGTH);

    int curDir1, curDir2;
    int isWalk1, isWalk2;
    sscanf(infos, "%d %d %d %d %d %d %d %d %d %d %d %d", &player1->iframe, &isWalk1, &curDir1, 
        &player1->cpt_reset, &player1->rect.x, &player1->rect.y, &player2->iframe, &isWalk2,
       &curDir2, &player2->cpt_reset, &player2->rect.x, &player2->rect.y);
    player1->curDir = curDir1;
    player2->curDir = curDir2;
    player1->isWalking = isWalk1;
    player2->isWalking = isWalk2;
    printf("Iframe : %d %d\n", player1->iframe, player2->iframe);
    change_direction(player1, curDir1, 0);
    change_direction(player2, curDir2, 1);
    return 0;
}

int join_server(SDL_Window* window, SDL_Renderer *render)
{
    IPaddress ip;           // Adresse IP du serveur
    TCPsocket serverSocket; // Socket du client

    SDLNet_ResolveHost(&ip, "127.0.0.1", 1234); // Se connecter au serveur a l'adresse IP 127.0.0.1 sur le port 1234
    serverSocket = SDLNet_TCP_Open(&ip);        // Ouvrir une connexion TCP avec le serveur

    SDL_Thread *sendContThread = SDL_CreateThread(sendControls, "SendControls", (void *)serverSocket); // Créer un thread pour recevoir les messages du serveur


    Map map;
    get_map_first(serverSocket, &map);
    SDL_SetWindowSize(window, map.size * TILE_SIZE, map.size * TILE_SIZE);

    const short nPlayers = 2;
    Player players[nPlayers];
    Key controls1[5] = {K_z, K_d, K_s, K_q, K_SPACE};
    Key controls2[5] = {K_UP, K_RIGHT, K_DOWN, K_LEFT, K_RSHIFT};
    init_player(&players[0], 1, 1, controls1, 0);
    init_player(&players[1], map.size - 2, map.size - 2, controls2, 1);

    // Load and display map and players
    display_map(render, &map);
    for (int i = 0; i < nPlayers; i++)
        display_player(render, &players[i]);
    SDL_RenderPresent(render);

    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;
    double deltaTime = 0;
    double targetfps = 1.0 / 60.0;
    double accumulator = 0.0;

    char timerStr[8];
    SDL_Color White = {255, 255, 255};
    TTF_Font *sans = TTF_OpenFont("../assets/nasa.ttf", 12);
    if (sans == NULL)
    {
        printf("Error while loading font: %s\n", TTF_GetError());
        return QUIT;
    }
    struct timespec start_time;
    clock_gettime(CLOCK_REALTIME, &start_time);
    SDL_Rect Message_rect; // create a rect
    int ww, wh;
    SDL_GetWindowSize(window, &ww, &wh);
    Message_rect.w = 50;                          // controls the width of the rect
    Message_rect.h = 50;                          // controls the height of the rect
    Message_rect.y = 0;                           // controls the rect's y coordinte
    Message_rect.x = ww / 2 - Message_rect.w / 2; // controls the rect's x coordinate

    SDL_Event event;
    bool done = false;
    while (!done) 
    {
        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();
        deltaTime = (double)((NOW - LAST) / (double)SDL_GetPerformanceFrequency()); // delta time in seconds
        accumulator += deltaTime;

        int eventPresent = SDL_PollEvent(&event);
        if (eventPresent)
        {
            if (event.type == SDL_QUIT)
                done = true;
        }
        while (accumulator > targetfps)
        {
            for (int iPlayer = 0; iPlayer < nPlayers; iPlayer++)
            {
                if (!players[iPlayer].isAlive)
                    continue;
                Player *curPlayer = &players[iPlayer];
                get_bonus(curPlayer, &map);
            }
            get_map(serverSocket, &map);
            get_player_infos(serverSocket, &players[0], &players[1]);
            //update_bombs_positions(window, &map, targetfps);
            accumulator -= targetfps;
        }
        struct timespec cur_time;
        clock_gettime(CLOCK_REALTIME, &cur_time);
        double dt = (cur_time.tv_sec - start_time.tv_sec) + (double)(cur_time.tv_nsec - start_time.tv_nsec) / (double)BILLION;
        sprintf(timerStr, "%.2f", dt);
        SDL_Surface *surfaceMessage = TTF_RenderText_Solid(sans, timerStr, White);
        SDL_Texture *message = SDL_CreateTextureFromSurface(render, surfaceMessage);

        SDL_RenderClear(render);
        display_map(render, &map);
        /*display_bombs(render, &map);
        for (int b = 0; b < MAX_BOMBS && bombs[b] != NULL; b++)
        {
            SDL_RenderDrawRect(render, &bombs[b]->collision_rect);
        }*/
        for (int i = 0; i < nPlayers; i++)
        {
            updateDeathStatus(&map, &players[i]);
            if (players[i].isAlive)
            {
                display_player(render, &players[i]);
            }
        }
        SDL_RenderCopy(render, message, NULL, &Message_rect);
        SDL_RenderPresent(render);
    }
    
    SDL_WaitThread(sendContThread, NULL); // Attendre la fin du thread de réception
    SDLNet_TCP_Close(serverSocket);
    return 0;
}