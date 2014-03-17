#include <SDL/SDL.h>
#include <assert.h>
#include <constant.h>
#include <game.h>
#include <window.h>
#include <keyboard.h>
#include <menu.h>


int main(int argc, char *argv[]) {

	window_create(SIZE_BLOC * MAP_WIDTH,
			SIZE_BLOC * MAP_HEIGHT + BANNER_HEIGHT + LINE_HEIGHT);

	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

	// to obtain the DEFAULT_GAME_FPS, we have to reach a loop duration of (1000 / DEFAULT_GAME_FPS) ms
	int ideal_speed = 1000 / DEFAULT_GAME_FPS;
	int timer, execution_speed;
	enum state state = NEWGAME;
	struct game* game = NULL;
	struct menu* menu = NULL;
	void* thing = NULL;

	// game loop
	// fixed time rate implementation
	int done = 0;

	while (!done) {
		timer = SDL_GetTicks();

		switch(state){
		case NEWGAME:
			//if(menu != NULL)
				//menu_free(menu);
			game = game_new();
			thing = game;
			state = GAME;
			/* no break */
		case GAME:
			assert(game);
			game_display(game);
			break;
		case ENDGAME:
			//if(game != NULL)
				//game_free(game);
			//menu = new_menu(MAIN);
			state = QUIT;
			/* no break */
		case MENU:
			//assert(menu);
			//menu_display(menu);
			break;
		case QUIT:
			//if(game != NULL)
				//game_free(game);
			//if(menu != NULL)
				//menu_free(menu);
			done = 1;
			/* no break */
		}
		if(thing != NULL)
			state = input_keyboard(thing, state);

		execution_speed = SDL_GetTicks() - timer;
		if (execution_speed < ideal_speed)
			SDL_Delay(ideal_speed - execution_speed); // we are ahead of ideal time. let's wait.
	}
	game_free(game);
	window_free();
	SDL_Quit();

	return EXIT_SUCCESS;
}
