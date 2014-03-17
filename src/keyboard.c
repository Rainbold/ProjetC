#include <assert.h>
#include <constant.h>
#include <SDL/SDL.h>
#include <game.h>
#include <keyboard.h>
#include <menu.h>

enum state input_keyboard(void* thing, enum state state) { // state : 0 = MENU, 1 = GAME
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT: // keyboard : esc
			return ENDGAME;
		case SDL_KEYDOWN:
			switch (state) {
			case GAME:
				assert(thing);
				struct game* game = thing;
				return(game_update(game, event.key.keysym.sym));
				break;
			case MENU:
				assert(thing);
				struct menu* menu = thing;
				return(menu_update(menu, event.key.keysym.sym));
				break;
			default:
				break;
			}
			break;
		}
	}
	return state;
}
