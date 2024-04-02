#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


int main(int argc, char * argv[]) {
	int exit_status = EXIT_FAILURE;

	if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) != 0) {
		printf("Erreur lors de l'init\n");
		goto Quit;
	}

	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	if (SDL_CreateWindowAndRenderer(640, 480, SDL_WINDOW_SHOWN, &window, &renderer) != 0) {
		printf("Error: %s\n", SDL_GetError());
		goto Quit;
	}
	SDL_SetWindowTitle(window, "Bomberman");

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_Rect rect = {10, 10, 100, 100};
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_RenderDrawRect(renderer, &rect);

	SDL_RenderPresent(renderer);

	SDL_Delay(3000);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	exit_status = EXIT_SUCCESS;

Quit:
	SDL_Quit();
	return exit_status;
}
