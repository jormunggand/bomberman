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

    SDL_Surface* tmp = IMG_Load("bomberman-sprite.png");
    if (tmp == NULL) {
        printf("%s", IMG_GetError());
    }
    SDL_Texture* player = SDL_CreateTextureFromSurface(render, tmp);
    SDL_FreeSurface(tmp);

    SDL_Rect dst = {120, 60, 0, 0};
    SDL_QueryTexture(player, 0, 0, &dst.w, &dst.h);

    SDL_RenderCopy(render, player, NULL, &dst);

    SDL_RenderPresent(render);
    SDL_Delay(3000);

    //SDL_FreeSurface(surf);
    //SDL_DestroyTexture(sprite);
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    exit_status = EXIT_SUCCESS;

Quit:
    SDL_Quit();
    return exit_status;
}