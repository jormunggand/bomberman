#include "keyboard.h"

void initHandler(KeyboardHandler* self) {
    for (int k = 0; k < N_KEYS; k++)
        self->keyState[k] = SDL_RELEASED;
}

void handleEvent(KeyboardHandler* self, SDL_KeyboardEvent event) {
    Key key = NONE;
    switch (event.keysym.sym) {
        case (SDLK_ESCAPE):
            key = K_ESC;
            break;
        case (SDLK_UP):
        case (SDLK_z):
            key = K_UP;
            break;
        case (SDLK_RIGHT):
        case (SDLK_d):
            key = K_RIGHT;
            break;
        case(SDLK_DOWN):
        case (SDLK_s):
            key = K_DOWN;
            break;
        case (SDLK_LEFT):
        case (SDLK_q):
            key = K_LEFT;
            break;
        case (SDLK_SPACE):
            key = K_SPACE;
            break;
        default:
            break;
    }
    if (key != NONE)
        self->keyState[key] = event.state;
}

bool anyDirectionPressed(KeyboardHandler* self) {
    return (self->keyState[K_UP] == SDL_PRESSED || self->keyState[K_RIGHT] == SDL_PRESSED 
        || self->keyState[K_DOWN] == SDL_PRESSED || self->keyState[K_LEFT] == SDL_PRESSED);
}