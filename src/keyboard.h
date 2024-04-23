#ifdef __linux__
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#elif __APPLE__
#include <SDL.h>
#include <SDL_image.h>
#endif

#include <stdbool.h>

#define N_KEYS (10)

typedef enum Key{
    NONE = -1,
    K_ESC = 0,
    K_UP = 1,
    K_RIGHT = 2,
    K_DOWN = 3,
    K_LEFT = 4,
    K_SPACE = 5,
    K_g = 6
} Key;


typedef struct KeyboardHandler {
    int keyState[N_KEYS];
} KeyboardHandler;


void initHandler(KeyboardHandler* self);
void handleEvent(KeyboardHandler* self, SDL_KeyboardEvent event);
bool anyDirectionPressed(KeyboardHandler* self);