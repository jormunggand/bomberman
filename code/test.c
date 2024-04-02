#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


int main() {
	printf("BOMBERMAN\n");
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		printf("Erreur\n");
	return 0;
}
