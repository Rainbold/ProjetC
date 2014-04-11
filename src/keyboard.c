#include <assert.h>
#include <constant.h>
#include <SDL/SDL.h>
#include <game.h>
#include <keyboard.h>
#include <menu.h>
#include <wiimote.h>

enum state input_keyboard(void* thing, enum state state) { // state : 0 = MENU, 1 = GAME
	SDL_Event event;
	key_event_t key_event = DOWN;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT: // keyboard : esc
			return QUIT;
		case SDL_KEYUP:
			key_event = UP;
			/* no break */
		case SDL_KEYDOWN:
#ifdef USE_WIIMOTE
			if(key_event == DOWN && event.key.keysym.sym == SDLK_F12)
				wiimote_link(2);
#endif
			switch (state) {
			case GAME:
				assert(thing);
				struct game* game = thing;
				return(game_update(game, event.key.keysym.sym, key_event));
				break;
			case MENU:
				assert(thing);
				struct menu* menu = thing;
				return(menu_update(menu, event.key.keysym.sym, key_event));
				break;
			default:
				break;
			}
			break;
		}
	}
	return state;
}
