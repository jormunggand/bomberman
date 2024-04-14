#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h> 
#include <SDL_image.h>


int init(SDL_Window **window, SDL_Renderer **renderer, int w, int h)
{
    if(0 != SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        return -1;
    }
    if(0 != SDL_CreateWindowAndRenderer(w, h, SDL_WINDOW_SHOWN, window, renderer))
    {
        fprintf(stderr, "Erreur SDL_CreateWindowAndRenderer : %s", SDL_GetError());
        return -1;
    }
    return 0;
}

SDL_Texture *loadImage(const char path[], SDL_Renderer *renderer)
{
    SDL_Surface *tmp = NULL; 
    SDL_Texture *texture = NULL;
    tmp = IMG_Load(path);
    if(NULL == tmp)
    {
        fprintf(stderr, "Erreur IMG_Load : %s", SDL_GetError());
        return NULL;
    }
    texture = SDL_CreateTextureFromSurface(renderer, tmp);
    SDL_FreeSurface(tmp);
    if(NULL == texture)
    {
        fprintf(stderr, "Erreur SDL_CreateTextureFromSurface : %s", SDL_GetError());
        return NULL;
    }
    return texture;
}

int setWindowColor(SDL_Renderer *renderer, SDL_Color color)
{
    if (SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a) < 0)
        return -1;
    if (SDL_RenderClear(renderer) < 0)
        return -1;
    return 0;  
}

int main(){
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *image = NULL;
    int statut = EXIT_FAILURE;
    SDL_Color blanc = {255, 255, 255, 255};
    if(0 != init(&window, &renderer, 640, 480)) 
        goto Quit;
    
    setWindowColor(renderer, blanc);
    image = loadImage("Terrain_blocks.png", renderer); 
    if(NULL == image)
        goto Quit;

    statut = EXIT_SUCCESS;

    SDL_RenderCopy(renderer, image, NULL, NULL);
    SDL_RenderPresent(renderer);

    bool is_running = true;
    SDL_Event event;
    while (is_running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                is_running = false;
            }
        }
        SDL_Delay(16);
    }
    Quit:
    if (NULL != image)
        SDL_DestroyTexture(image);
    if (NULL != renderer)
        SDL_DestroyRenderer(renderer);
    if (NULL != window)
        SDL_DestroyWindow(window);
    SDL_Quit();
    return statut;
}