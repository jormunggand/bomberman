#include <utils.h>


int width = 100; // size of each tile in pixels

int main(int argc, char* argv[]) {
    int exit_status = EXIT_FAILURE;

    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) != 0) {
        printf("%s", SDL_GetError());
        goto Quit;
    }
    
    SDL_Window* window = NULL;
    SDL_Renderer* render = NULL;
    if (SDL_CreateWindowAndRenderer(8*width, 8*width, SDL_WINDOW_SHOWN, &window, &render) != 0) {
        printf("%s", SDL_GetError());
        goto Quit;
    }
    SDL_SetWindowTitle(window, "Damier");

    SDL_Color black = {0, 0, 0, 255};
    SDL_Color white = {255, 255, 255, 255};
    SDL_Rect rect = {0, 0, width, width};

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            rect.x = i*width;
            rect.y = j*width;
            if ((i + j) % 2 == 0)
                SDL_SetRenderDrawColor(render, white.r, white.g, white.b, white.a);
            else
                SDL_SetRenderDrawColor(render, black.r, black.g, black.b, black.a);
            SDL_RenderFillRect(render, &rect);
        }
    }
    SDL_RenderPresent(render);
    SDL_Delay(3000);

    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    exit_status = EXIT_SUCCESS;


Quit:
    SDL_Quit();
    return exit_status;
}