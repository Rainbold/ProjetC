#include <SDL/SDL.h>

#include <constant.h>
#include <game.h>
#include <window.h>

int main(int argc, char *argv[]) {
	struct game* game = game_new();

	window_create(SIZE_BLOC * MAP_WIDTH,
			SIZE_BLOC * MAP_HEIGHT + BANNER_HEIGHT + LINE_HEIGHT);

	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

	// to obtain the DEFAULT_GAME_FPS, we have to reach a loop duration of (1000 / DEFAULT_GAME_FPS) ms
	int ideal_speed = 1000 / DEFAULT_GAME_FPS;
	int timer, execution_speed;

	// game loop
	// fixed time rate implementation
	int done = 0;
	while (!done) {
		timer = SDL_GetTicks();

		done = game_update(game);
		game_display(game);

		execution_speed = SDL_GetTicks() - timer;
		if (execution_speed < ideal_speed)
			SDL_Delay(ideal_speed - execution_speed); // we are ahead of ideal time. let's wait.
	}

	window_free();
	game_free(game);
	SDL_Quit();

	return EXIT_SUCCESS;
}
