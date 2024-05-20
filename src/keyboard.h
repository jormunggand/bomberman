#ifdef __linux__
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#elif __APPLE__
#include <SDL.h>
#include <SDL_image.h>
#endif

#include <stdbool.h>

#define N_KEYS (15)

typedef enum {
    K_NONE = -1,
    K_ESC = 0,
    K_UP = 1,
    K_RIGHT = 2,
    K_DOWN = 3,
    K_LEFT = 4,
    K_SPACE = 5,
    K_z = 6,
    K_q = 7,
    K_s = 8,
    K_d = 9,
    K_RSHIFT = 10
} Key;


// KeyboardHandler is a structure that stores the state of the keys (up or down)
typedef struct KeyboardHandler {
    int keyState[N_KEYS];
} KeyboardHandler;

// set all keys to released

int sdl_to_k(int SDL_key);
void initHandler(KeyboardHandler* self);

// update the state of the keys according to the event received
void handleEvent(KeyboardHandler* self, SDL_KeyboardEvent event);

// check if any direction key is pressed for the given player
bool anyDirectionPressed(KeyboardHandler* self, int iPlayer);