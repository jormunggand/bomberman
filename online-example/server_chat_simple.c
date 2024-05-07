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
        if (SDLNet_TCP_Recv(socket, message, MAX_MESSAGE_LENGTH) > 0) // Recevoir un message du client
        {
            printf("Reçu: %s\n", message); // Affiche le message reçu du client
        }
    }
    return 0;
}

int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_EVERYTHING); // Initialiser SDL
    SDLNet_Init();                 // Initialiser SDL_net

    IPaddress ip;                         // Adresse IP du serveur
    TCPsocket serverSocket, clientSocket; // Sockets du serveur et du client

    SDLNet_ResolveHost(&ip, NULL, 1234); // Définir le serveur pour écouter sur le port 1234
    serverSocket = SDLNet_TCP_Open(&ip); // Ouvrir un socket TCP pour le serveur

    printf("En attente de connexion...\n");

    do
    {
        clientSocket = SDLNet_TCP_Accept(serverSocket); // Accepter une connexion entrante
        SDL_Delay(250);                                 // Attendre un peu avant de vérifier à nouveau
    } while (!clientSocket); // Attendre qu'un client se connecte

    printf("Client connecté.\n");

    // Créer un thread pour recevoir les messages du client
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

    SDL_WaitThread(recvThread, NULL); // Attendre que le thread de réception se termine
    SDLNet_TCP_Close(clientSocket);   // Fermer la connexion avec le client
    SDLNet_TCP_Close(serverSocket);   // Fermer le socket du serveur
    SDLNet_Quit();                    // Quitter SDL_net
    SDL_Quit();                       // Quitter SDL
    return 0;
}
