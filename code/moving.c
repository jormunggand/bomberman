#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "utils.h"

void edge_collision(SDL_Window* window, SDL_Rect* rect) {
    int width, height;
    SDL_GetWindowSize(window, &width, &height);
    if (rect->x < 0)
        rect->x = 0;
    if (rect->y < 0)
        rect->y = 0;
    if (rect->x + rect->w > width)
        rect->x = width-rect->w;
    if (rect->y + rect->h > height)
        rect->y = height-rect->h;
}

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

    SDL_Texture* player = loadImage("../assets/bomberman-sprite.png", render);
    SDL_Rect dst = {320,320,0,0};
    SDL_QueryTexture(player, 0, 0, &dst.w, &dst.h);

    SDL_RenderCopy(render, player, NULL, &dst);
    SDL_RenderPresent(render);
    

    int velocity = 58;
    SDL_Event event;
    bool done = false;
    while (!done) {
        SDL_WaitEvent(&event);
        if (event.type == SDL_QUIT)
            done = true;
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_DOWN)
                dst.y += velocity;
            else if (event.key.keysym.sym == SDLK_UP)
                dst.y -= velocity;
            else if (event.key.keysym.sym == SDLK_RIGHT)
                dst.x += velocity;
            else if (event.key.keysym.sym == SDLK_LEFT)
                dst.x -= velocity;

            edge_collision(window, &dst);
            SDL_RenderClear(render);
            SDL_RenderCopy(render, player, NULL, &dst);
            SDL_RenderPresent(render);
        }
    }

    SDL_DestroyTexture(player);
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    exit_status = EXIT_SUCCESS;

Quit:
    SDL_Quit();
    return exit_status;
}