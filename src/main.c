#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <assert.h>
#include <constant.h>
#include <game.h>
#include <window.h>
#include <keyboard.h>
#include <menu.h>
#include <sprite.h>
#include <wiimote.h>
#include <multi.h>

int main(int argc, char *argv[]) {

	window_create(MENU_WIDTH, MENU_HEIGHT);

	//SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

	// to obtain the DEFAULT_GAME_FPS, we have to reach a loop duration of (1000 / DEFAULT_GAME_FPS) ms
	int ideal_speed = 1000 / DEFAULT_GAME_FPS;
	int timer, execution_speed;
	enum state state = ENDGAME;
	struct game* game = NULL;

	sprite_load(); // load sprites into process memory

#ifdef USE_WIIMOTE
	// Starting the link with remote
	wiimote_init();
#endif

	// game loop
	// fixed time rate implementation
	int done = 0;
	int nb_players = 1;

	while (!done) {
		timer = SDL_GetTicks();

		switch(state) {
		case NEWGAME_SINGLE:
			menu_free();
			game = game_new(0, 1); // lvl 0, 1 player
			state = GAME;
			break;

		case NEWGAME_MULTI4:
			nb_players++;
			/* no break */
		case NEWGAME_MULTI3:
			nb_players++;
			/* no break */
		case NEWGAME_MULTI2:
			nb_players++;

			menu_free();
			game = game_new(0, nb_players); // lvl 0, 1 player
			game_set_state(game, CHOOSE_MAP);
			state = GAME;
			break;

		case GAME:
			if(nb_players == 1)
				game_display(game);
			else
				multi_display(game);
			break;

		case ENDGAME:
			if(game != NULL) {
				game_free(game);
				game = NULL;
			}
			menu_free();
			new_menu(MAIN);
			window_resize(MENU_WIDTH, MENU_HEIGHT);
			state = -1;
			nb_players = 1;
			break;

		case QUIT:
			if(game != NULL) {
				game_free(game);
				game = NULL;
			}
			menu_free();
			done = 1;
			break;
		default:
			menu_display(MENU_WIDTH / 2, MENU_HEIGHT / 2);
			break;
		}

			state = input_keyboard(game, state);

#ifdef USE_WIIMOTE
			state = input_wiimote(game, state);
#endif
		execution_speed = SDL_GetTicks() - timer;
		if (execution_speed < ideal_speed)
			SDL_Delay(ideal_speed - execution_speed); // we are ahead of ideal time. let's wait.
	}

	window_free();
	TTF_Quit();
	SDL_Quit();

	return EXIT_SUCCESS;
}
