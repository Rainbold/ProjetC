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

	window_create(MENU_WIDTH, MENU_HEIGHT); // Create a windows of MENU_WIDTH * MENU_HEIGHT

	// to obtain the DEFAULT_GAME_FPS, we have to reach a loop duration of (1000 / DEFAULT_GAME_FPS) ms
	int ideal_speed = 1000 / DEFAULT_GAME_FPS;
	int timer, execution_speed;

	enum state state = ENDGAME; // State of the programme
	struct game* game = NULL;

	sprite_load(); // load sprites into process memory

#ifdef USE_WIIMOTE // USE_WIIMOTE is defined in constant.h
	// Starting the wiimote module
	wiimote_init();
#endif

	// game loop
	int done = 0; // Used to quit
	int nb_players = 1; // number of players

	while (!done) {
		timer = SDL_GetTicks(); // timer used to obtain the DEFAULT_GAME_FPS
		
		switch(state) {
		case NEWGAME_SINGLE: // newgame with 1 player
			menu_free(); // delete the menu to display the game
			game = game_new(0, 1); // new game : lvl 0 and 1 player
			state = GAME;
			break;
		case LOADGAME: // load game from a savegame
			menu_free();
			game = game_new(-1, 1); // lvl -1, to load data
			state = GAME;
			break;

		case NEWGAME_MULTI4: // Multiplayer game
			nb_players++;
			/* no break */
		case NEWGAME_MULTI3:
			nb_players++;
			/* no break */
		case NEWGAME_MULTI2:
			nb_players++;

			menu_free();
			game = game_new(0, nb_players); // lvl 0, nb_players players
			game_set_state(game, CHOOSE_MAP); // change the state of the game
			state = GAME;
			break;

		case GAME:
			if(nb_players == 1)
				game_display(game);
			else if(nb_players >= 2 && nb_players <= 4)
				multi_display(game);
			else {
				printf("Error with nb_players in main.c\n");
				state = ENDGAME;
			}
			break;

		case GO :
			if(game != NULL) {
				game_free(game);
				game = NULL;
			}
			menu_free();
			new_menu(GAMEOVER); // Create a new menu
			window_resize(MENU_WIDTH, MENU_HEIGHT);
			state = DEFAULT;
			nb_players = 1;
			break;

		case W :
			if(game != NULL) {
				game_free(game);
				game = NULL;
			}
			menu_free();
			new_menu(WIN); // Create a new menu
			window_resize(MENU_WIDTH, MENU_HEIGHT);
			state = DEFAULT;
			nb_players = 1;
			break;

		case ENDGAME:
			if(game != NULL) {
				game_free(game);
				game = NULL;
			}
			menu_free();
			new_menu(MAIN); // Create a new menu
			window_resize(MENU_WIDTH, MENU_HEIGHT);
			state = DEFAULT;
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

		// fixed time rate implementation
		execution_speed = SDL_GetTicks() - timer;
		if (execution_speed < ideal_speed)
			SDL_Delay(ideal_speed - execution_speed); // we are ahead of ideal time. let's wait.
	}

	sprite_free();
	window_free();
	TTF_Quit();
	SDL_Quit();

	return EXIT_SUCCESS;
}
