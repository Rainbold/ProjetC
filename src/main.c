#include <SDL/SDL.h>
#include <assert.h>
#include <constant.h>
#include <game.h>
#include <window.h>
#include <keyboard.h>
#include <menu.h>
#include <sprite.h>

int main(int argc, char *argv[]) {

	window_create(SIZE_BLOC * MAP_WIDTH,
			SIZE_BLOC * MAP_HEIGHT + BANNER_HEIGHT + LINE_HEIGHT);

	//SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

	// to obtain the DEFAULT_GAME_FPS, we have to reach a loop duration of (1000 / DEFAULT_GAME_FPS) ms
	int ideal_speed = 1000 / DEFAULT_GAME_FPS;
	int timer, execution_speed;
	enum state state = ENDGAME;
	struct game* game = NULL;
	struct menu* menu = NULL;
	void* thing = NULL;

	sprite_load(); // load sprites into process memory

	// game loop
	// fixed time rate implementation
	int done = 0;

	while (!done) {
		timer = SDL_GetTicks();

		switch(state){
		case NEWGAME:
			if(menu != NULL) {
				menu_free(menu);
				menu = NULL;
			}
			game = game_new();
			thing = game;
			state = GAME;
			/* no break */
		case GAME:
			assert(game);
			game_display(game);
			break;
		case ENDGAME:
			if(game != NULL) {
				game_free(game);
				game = NULL;
			}
			menu = new_menu(MAIN);
			thing = menu;
			state = MENU;
			/* no break */
		case MENU:
			assert(menu);
			menu_display(menu);
			break;
		case QUIT:
			if(game != NULL) {
				game_free(game);
				game = NULL;
			}
			if(menu != NULL) {
				menu_free(menu);
				menu = NULL;
			}
			done = 1;
			break;
		}
		if(thing != NULL)
			state = input_keyboard(thing, state);

		execution_speed = SDL_GetTicks() - timer;
		if (execution_speed < ideal_speed)
			SDL_Delay(ideal_speed - execution_speed); // we are ahead of ideal time. let's wait.
	}

	window_free();
	SDL_Quit();

	return EXIT_SUCCESS;
}
