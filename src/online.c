#include "online.h"

void host_server(SDL_Renderer* render, int windowWidth, int windowHeight) {
    IPaddress ip; // server's ip address
    TCPsocket serverSocket, clientScoket;

    SDLNet_ResolveHost(&ip, NULL, 1234); // listen on port 1234
    serverSocket = SDLNet_TCP_Open(&ip);

    // Draw waiting screen
    SDL_SetRenderDrawColor(render, 37, 150,190, 255);
    SDL_RenderClear(render);

    TTF_Font* nasa = TTF_OpenFont("../assets/Fonts/nasa.ttf", 24);

    SDL_RenderPresent(render);

    do {
        clientScoket = SDLNet_TCP_Accept(serverSocket);
        SDL_Delay(250);
    } while (!clientScoket);
    
    return;
}

void join_server(SDL_Renderer* render, int windowWidth, int windowHeight) {
    return;
}