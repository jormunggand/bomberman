#include "online.h"

#include <stdbool.h>

#define MAX_MESSAGE_LENGTH 1024

int receiveMessages(void *data)
{
    TCPsocket socket = (TCPsocket)data; // Casting de data en TCPsocket
    char message[MAX_MESSAGE_LENGTH];   // Buffer pour stocker les messages reçus

    while (1)
    {
        if (SDLNet_TCP_Recv(socket, message, MAX_MESSAGE_LENGTH) > 0) // Recevoir un message du client
        {
            printf("Reçu: %s\n", message); // Affiche le message reçu du client
        }
    }
    return 0;
}

/*int sendControls(void* data) 
{
    TCPsocket socket = (TCPsocket)data; // Casting de data en TCPsocket
    char message[MAX_MESSAGE_LENGTH];

    SDL_Event event;
    bool done = false;
    while (!done) 
    {
        if (SDL_PollEvent(&event))
        {
            if (event.type == SDL_KEYDOWN) {
                event.key.keysym.scancode
            }
        }
    }

}*/

int host_server(SDL_Renderer* render, int windowWidth, int windowHeight) {
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

    TTF_Font* nasa = TTF_OpenFont("../assets/Fonts/nasa.ttf", 24);
    if (nasa == NULL) 
    {
        printf("%s\n", TTF_GetError());
        return -1;
    }
    SDL_Color red = {.r=255, .g=0, .b=0, .a=255};
    SDL_Surface* surf = TTF_RenderText_Solid(nasa, "En attente du joueur 2...", red);
    SDL_Texture* mess = SDL_CreateTextureFromSurface(render, surf);
    SDL_Rect mess_rect = {.w=200, .h=50, .x=windowWidth/2 - 100, .y=windowHeight/2 - 50};

    SDL_RenderCopy(render, mess, NULL, &mess_rect);
    SDL_RenderPresent(render);

    // Wait for a connection
    SDL_Event event;
    Uint64 last = 0, now;
    int delta;
    while (clientSocket == NULL) 
    {
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
            return 0;

        now = SDL_GetPerformanceCounter();
        delta = now-last;
        if (delta > 250) // wait 250ms before attempting a new connection
        { 
            clientSocket = SDLNet_TCP_Accept(serverSocket);
            last = now;
        }

    } 
    printf("Client connected\n");
    

    SDL_Thread *recvThread = SDL_CreateThread(receiveMessages, "ReceiveMessages", (void *)clientSocket);
    char message[MAX_MESSAGE_LENGTH]; // Buffer pour stocker les messages entrés par l'utilisateur

    while (1)
    {
        fgets(message, MAX_MESSAGE_LENGTH, stdin); // Lire l'entrée utilisateur

        if (SDLNet_TCP_Send(clientSocket, message, strlen(message) + 1) < strlen(message) + 1) // Envoyer le message au client
        {
            fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError()); // Afficher une erreur si l'envoi échoue
            break;
        }
    }

    SDL_WaitThread(recvThread, NULL);

    SDLNet_TCP_Close(clientSocket);
    SDLNet_TCP_Close(serverSocket);
    return 0;

NetworkError:
    printf("Network Error: %s\n", SDLNet_GetError());
    return -1;
}

int join_server(SDL_Renderer* render, int windowWidth, int windowHeight) {
    IPaddress ip;           // Adresse IP du serveur
    TCPsocket clientSocket; // Socket du client

    SDLNet_ResolveHost(&ip, "127.0.0.1", 1234); // Se connecter au serveur a l'adresse IP 127.0.0.1 sur le port 1234
    clientSocket = SDLNet_TCP_Open(&ip);        // Ouvrir une connexion TCP avec le serveur

    SDL_Thread *recvThread = SDL_CreateThread(receiveMessages, "ReceiveMessages", (void *)clientSocket); // Créer un thread pour recevoir les messages du serveur

    char message[MAX_MESSAGE_LENGTH]; // Buffer pour stocker les messages entrés par l'utilisateur

    while (1)
    {
        fgets(message, MAX_MESSAGE_LENGTH, stdin); // Lire l'entrée utilisateur

        if (SDLNet_TCP_Send(clientSocket, message, strlen(message) + 1) < strlen(message) + 1) // Envoyer le message au serveur
        {
            fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError()); // Afficher une erreur si l'envoi échoue
            break;
        }
    }

    SDL_WaitThread(recvThread, NULL); // Attendre la fin du thread de réception
    SDLNet_TCP_Close(clientSocket);
    return 0;
}