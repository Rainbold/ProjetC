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

	while (!done) {
		timer = SDL_GetTicks();

		switch(state) {
		case NEWGAME_SINGLE:
			menu_free();
			game = game_new(0, 1); // lvl 0
			state = GAME;
			break;

		case NEWGAME_MULTI2_1:
			menu_free();
			game = game_new(0, 2); // lvl 0
			state = GAME;
			break;
		case NEWGAME_MULTI2_2:
			menu_free();
			game = game_new(1, 2); // lvl 0
			state = GAME;
			break;
		case NEWGAME_MULTI2_3:
			menu_free();
			game = game_new(2, 2); // lvl 0
			state = GAME;
			break;
		case NEWGAME_MULTI2_4:
			menu_free();
			game = game_new(3, 2); // lvl 0
			state = GAME;
			break;
		case NEWGAME_MULTI2_5:
			menu_free();
			game = game_new(4, 2); // lvl 0
			state = GAME;
			break;
		case NEWGAME_MULTI2_6:
			menu_free();
			game = game_new(5, 2); // lvl 0
			state = GAME;
			break;
		case NEWGAME_MULTI2_7:
			menu_free();
			game = game_new(6, 2); // lvl 0
			state = GAME;
			break;
		case NEWGAME_MULTI2_8:
			menu_free();
			game = game_new(7, 2); // lvl 0
			state = GAME;
			break;
		case NEWGAME_MULTI2_9:
			menu_free();
			game = game_new(8, 2); // lvl 0
			state = GAME;
			break;
		case NEWGAME_MULTI2_10:
			menu_free();
			game = game_new(9, 2); // lvl 0
			state = GAME;
			break;

		case NEWGAME_MULTI3_1:
			menu_free();
			game = game_new(0, 3); // lvl 0
			state = GAME;
			break;
		case NEWGAME_MULTI3_2:
			menu_free();
			game = game_new(1, 3); // lvl 0
			state = GAME;
			break;
		case NEWGAME_MULTI3_3:
			menu_free();
			game = game_new(2, 3); // lvl 0
			state = GAME;
			break;
		case NEWGAME_MULTI3_4:
			menu_free();
			game = game_new(3, 3); // lvl 0
			state = GAME;
			break;
		case NEWGAME_MULTI3_5:
			menu_free();
			game = game_new(4, 3); // lvl 0
			state = GAME;
			break;
		case NEWGAME_MULTI3_6:
			menu_free();
			game = game_new(5, 3); // lvl 0
			state = GAME;
			break;
		case NEWGAME_MULTI3_7:
			menu_free();
			game = game_new(6, 3); // lvl 0
			state = GAME;
			break;
		case NEWGAME_MULTI3_8:
			menu_free();
			game = game_new(7, 3); // lvl 0
			state = GAME;
			break;
		case NEWGAME_MULTI3_9:
			menu_free();
			game = game_new(8, 3); // lvl 0
			state = GAME;
			break;
		case NEWGAME_MULTI3_10:
			menu_free();
			game = game_new(9, 3); // lvl 0
			state = GAME;
			break;

		case NEWGAME_MULTI4_1:
			menu_free();
			game = game_new(0, 4); // lvl 0
			state = GAME;
			break;
		case NEWGAME_MULTI4_2:
			menu_free();
			game = game_new(1, 4); // lvl 0
			state = GAME;
			break;
		case NEWGAME_MULTI4_3:
			menu_free();
			game = game_new(2, 4); // lvl 0
			state = GAME;
			break;
		case NEWGAME_MULTI4_4:
			menu_free();
			game = game_new(3, 4); // lvl 0
			state = GAME;
			break;
		case NEWGAME_MULTI4_5:
			menu_free();
			game = game_new(4, 4); // lvl 0
			state = GAME;
			break;
		case NEWGAME_MULTI4_6:
			menu_free();
			game = game_new(5, 4); // lvl 0
			state = GAME;
			break;
		case NEWGAME_MULTI4_7:
			menu_free();
			game = game_new(6, 4); // lvl 0
			state = GAME;
			break;
		case NEWGAME_MULTI4_8:
			menu_free();
			game = game_new(7, 4); // lvl 0
			state = GAME;
			break;
		case NEWGAME_MULTI4_9:
			menu_free();
			game = game_new(8, 4); // lvl 0
			state = GAME;
			break;
		case NEWGAME_MULTI4_10:
			menu_free();
			game = game_new(9, 4); // lvl 0
			state = GAME;
			break;



		case GAME:
			game_display(game);
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
