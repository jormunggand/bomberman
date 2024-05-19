// directives de préprocesseur pour determiner le système d'exploitation
#ifdef __APPLE__
// si on compile ce code sur macOS
#include <SDL.h>
#include <SDL_net.h>
#else
// si on compile ce code sur Linux ou Windows
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#endif
#include <stdio.h>
#include <string.h>

#define MAX_MESSAGE_LENGTH 1024 // Taille du buffer pour stocker les messages

/*
 * Function: receiveMessages
 * -------------------------
 *   Ecoute et affiche continuellement les messages reçus via un socket TCP.
 *
 *   Cette fonction est conçue pour être utilisée comme un thread séparé qui
 *   écoute indéfiniment un socket TCP donné pour des messages entrants. Chaque
 *   message reçu est immédiatement affiché sur la console. La fonction continue
 *   de recevoir et d'afficher des messages jusqu'à ce qu'une erreur se produise,
 *   indiquant potentiellement que la connexion a été interrompue ou est en état d'erreur.
 *
 *   data: un pointeur void qui doit être casté en TCPsocket. Ce pointeur représente
 *         le socket TCP à partir duquel les messages seront reçus.
 *
 *   returns: Retourne toujours 0. En pratique, cette fonction est censée tourner
 *            indéfiniment jusqu'à ce que la réception échoue ou que le socket soit fermé.
 */
int receiveMessages(void *data)
{
    TCPsocket socket = (TCPsocket)data; // Casting de data en TCPsocket
    char message[MAX_MESSAGE_LENGTH];   // Buffer pour stocker les messages reçus

    while (1)
    {
        if (SDLNet_TCP_Recv(socket, message, MAX_MESSAGE_LENGTH) > 0) // Recevoir un message du serveur
        {
            printf("Reçu: %s\n", message); // Affiche les messages reçus du serveur
        }
    }
    return 0;
}

int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_EVERYTHING); // Initialiser SDL
    SDLNet_Init();                 // Initialiser SDL_net

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
    SDLNet_TCP_Close(clientSocket);   // Fermer la connexion
    SDLNet_Quit();                    // Quitter SDL_net
    SDL_Quit();                       // Quitter SDL
    return 0;
}
