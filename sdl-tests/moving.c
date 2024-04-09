#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../code/utils.h"

int main(int argc, char* argv[]) {
    int exit_status = EXIT_FAILURE;
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) != 0) {
        printf("Erreur dans l'init %s", SDL_GetError());
        goto Quit;
    }

    SDL_Window* window = NULL;
    SDL_Renderer* render = NULL;
    if (SDL_CreateWindowAndRenderer(1000, 1000, SDL_WINDOW_SHOWN, &window, &render) != 0) {
        printf("%s", SDL_GetError());
        goto Quit;
    }
    SDL_SetWindowTitle(window, "Moving Bomberman");

    SDL_Texture* player = loadImage("bomberman-sprite.png", render);
    

    SDL_RenderPresent(render);

    SDL_DestroyTexture(player);
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    exit_status = EXIT_SUCCESS;

Quit:
    SDL_Quit();
    return exit_status;
}