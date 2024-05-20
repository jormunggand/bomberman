#ifdef __linux__
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#elif __APPLE__
#include <SDL.h>
#include <SDL_image.h>
#endif

#include <stdbool.h>

#define ANIMATION_FRAMES (8)

struct Map;
enum Key;

typedef enum {
    FRONT = 0,
    BACK = 1,
    RIGHT = 2,
    LEFT = 3
} SpriteDirection;


typedef struct Player {
    SpriteDirection curDir;
    bool isWalking;
    int iframe;
    int cpt_reset; // reset the player animation when idling

    SDL_Texture** animations;
    SDL_Rect rect;
    SDL_Rect collisionRect;
    SDL_Rect flameHitbox;

    int nMaxBombs; // the maximum number of bombs the player can fire simultaneously
    int nCurBombs; // the current number of bombs the player has lit
    float speed;
    int flamePower;

    bool isAlive;


    // keyboard controls to go Up, Right, Down, Left and to Fire
    int controls[5];
} Player;


// initializes a player structure at the given position in map coordinates
void init_player(Player* player, int x, int y, int* controls, int playerIndex);

// changes the direction of the player and updates the animation array used to display the player
void change_direction(Player* player, SpriteDirection newDir, int playerIndex);

// updates the sprite of the player to go to the next frame of the animation
void update_sprite(Player* player);

// displays the player on the screen
void display_player(SDL_Renderer* render, Player* player);

// auxiliar function to load the player textures
int load_player_aux(SDL_Renderer* render, char* base, SDL_Texture** textures, int playerIndex);

// loads all the textures used to display the players
int load_player_textures(SDL_Renderer* render, int playerIndex);

// update the player's position in the direction given by the deltas if there is no collision that prevents it 
void edge_collision(SDL_Window* window, Player* player, struct Map *map, int deltaX, int deltaY, double elapsedTime);

// check if the rect is colliding with a wall of the map
bool check_collision(SDL_Rect* r, struct Map *map);

/* check if the rect is colliding with a bomb and update the movement of the bomb accordingly
- the collidedWithBomb parameter represents if the player was already colliding with a bomb in the next frame, 
  it is useful because the collision is ignored when the player initially plants the bomb and is inside it
- the sign parameter is used to determine where the incoming bomb is coming from --> 1 for left or up, -1 for right or down 
*/
bool bomb_collision(SDL_Rect* r, int sign, bool collidedWithBomb);

