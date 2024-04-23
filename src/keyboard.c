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
        case (SDLK_SPACE):
            key = K_SPACE;
            break;
        default:
            break;
    }
    if (key != NONE)
        self->keyState[key] = event.state;
}