#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

int main(int argc, char* argv[]) {
    int exit_status = EXIT_FAILURE;
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) != 0) {
        printf("Erreur dans l'init %s", SDL_GetError());
        goto Quit;
    }

    SDL_Window* window = NULL;
    SDL_Renderer* render = NULL;
    if (SDL_CreateWindowAndRenderer(600, 480, SDL_WINDOW_SHOWN, &window, &render) != 0) {
        printf("%s", SDL_GetError());
        goto Quit;
    }
    SDL_SetWindowTitle(window, "");

    //SDL_Texture* sprite = SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 50, 50);

    //SDL_Surface* surf = SDL_CreateRGBSurface(0, 300, 200, 32, 0, 0, 0, 0);

    SDL_Surface* player = IMG_Load("bomberman-sprite.png");
    if (player == NULL) {
        printf("%s", IMG_GetError());
    }


    SDL_RenderPresent(render);

    //SDL_FreeSurface(surf);
    //SDL_DestroyTexture(sprite);
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    exit_status = EXIT_SUCCESS;

Quit:
    SDL_Quit();
    return exit_status;
}