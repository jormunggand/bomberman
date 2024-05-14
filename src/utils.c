#include "utils.h"


int max(int a, int b) {
    return a > b ? a : b;
}

int min(int a, int b) {
    return a < b ? a : b;
}

int init(SDL_Window **window, SDL_Renderer **renderer, int w, int h){
    if( 0 != SDL_Init(SDL_INIT_VIDEO)) 
    {
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        return -1;
    }
    if (0 != TTF_Init()) {
        fprintf(stderr, "Erreur TTF_Init : %s", TTF_GetError());
    }
    if (0 != SDLNet_Init())  
    {
        fprintf(stderr, "Erreur SDLNet_Init : %s", SDLNet_GetError());
        return -1;
    }


    if(0 != SDL_CreateWindowAndRenderer(w, h, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE, window, renderer))
    {
        fprintf(stderr, "Erreur SDL_CreateWindowAndRenderer : %s", SDL_GetError());
        return -1;
    }
    return 0;
}

SDL_Texture* loadImage(const char path[], SDL_Renderer *renderer){
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
    if(NULL == texture) {
        fprintf(stderr, "Erreur SDL_CreateTextureFromSurface : %s", SDL_GetError());
        return NULL;
    }
    return texture;
}

int setColor(SDL_Renderer *renderer, SDL_Color color){
    return SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

bool point_in_rect(SDL_Rect rect, int x, int y) {
    return x >= rect.x && (x <= rect.x + rect.w) && y >= rect.y && (y <= rect.y + rect.h);
} 