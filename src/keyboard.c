#include "keyboard.h"

void initHandler(KeyboardHandler* self) {
    for (int k = 0; k < N_KEYS; k++)
        self->keyState[k] = SDL_RELEASED;
}

void handleEvent(KeyboardHandler* self, SDL_KeyboardEvent event) {
    Key key = K_NONE;
    switch (event.keysym.sym) {
        case (SDLK_ESCAPE):
            key = K_ESC;
            break;
        case (SDLK_UP):
            key = K_UP;
            break;
        case (SDLK_RIGHT):
            key = K_RIGHT;
            break;
        case(SDLK_DOWN):
            key = K_DOWN;
            break;
        case (SDLK_LEFT):
            key = K_LEFT;
            break;
        case (SDLK_RSHIFT):
            key = K_RSHIFT;
            break;
        case (SDLK_z):
            key = K_z;
            break;
        case (SDLK_d):
            key = K_d;
            break;
        case (SDLK_s):
            key = K_s;
            break;
        case (SDLK_q):
            key = K_q;
            break;
        case (SDLK_SPACE):
            key = K_SPACE;
            break;
        default:
            break;
    }
    if (key != K_NONE)
        self->keyState[key] = event.state;
}

bool anyDirectionPressed(KeyboardHandler* self, int iPlayer) {
    if (iPlayer == 1) {
        return (self->keyState[K_z] == SDL_PRESSED || self->keyState[K_d] == SDL_PRESSED 
        || self->keyState[K_s] == SDL_PRESSED || self->keyState[K_q] == SDL_PRESSED);
    } else if (iPlayer == 2) {
        return (self->keyState[K_UP] == SDL_PRESSED || self->keyState[K_RIGHT] == SDL_PRESSED 
        || self->keyState[K_DOWN] == SDL_PRESSED || self->keyState[K_LEFT] == SDL_PRESSED);
    }
}