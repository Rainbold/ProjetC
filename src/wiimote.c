#include <stdlib.h>
#include <stdio.h>
#include <wiiuse.h>
#include <constant.h>
#include <SDL/SDL.h>
#include <assert.h>
#include <game.h>
#include <menu.h>

#ifdef USE_WIIMOTE

#define MAX_WIIMOTES 4
wiimote** wiimotes;

void wiimote_init() {
	wiimotes =  wiiuse_init(MAX_WIIMOTES);
}

void wiimote_link(int time) { // time in s to find wiimotes
	int found, connected;
	printf("Looking for wiimotes...(%d s)\n", time);
	wiimotes =  wiiuse_init(MAX_WIIMOTES);

	found = wiiuse_find(wiimotes, MAX_WIIMOTES, time);

	if (!found) {
		printf("No wiimotes found.\n");
	}

	connected = wiiuse_connect(wiimotes, MAX_WIIMOTES);

	if (connected) {
		printf("Connected to %i wiimotes (of %i found).\n", connected, found);
	} else {
		printf("Failed to connect to any wiimote.\n");
	}
	//printf("connnected: %d, found: %d\n", connected, found);

	wiiuse_set_leds(wiimotes[0], WIIMOTE_LED_1);
	wiiuse_set_leds(wiimotes[1], WIIMOTE_LED_2);
	wiiuse_set_leds(wiimotes[2], WIIMOTE_LED_3);
	wiiuse_set_leds(wiimotes[3], WIIMOTE_LED_4);

	wiiuse_rumble(wiimotes[0], 1);
	wiiuse_rumble(wiimotes[1], 1);
	wiiuse_rumble(wiimotes[2], 1);
	wiiuse_rumble(wiimotes[3], 1);

	SDL_Delay(200);

	wiiuse_rumble(wiimotes[0], 0);
	wiiuse_rumble(wiimotes[1], 0);
	wiiuse_rumble(wiimotes[2], 0);
	wiiuse_rumble(wiimotes[3], 0);
}

enum state wiimote_send_event(enum state state, struct game* game, SDLKey key, key_event_t key_event) {
	switch (state) {
	case GAME:
		assert(game);
		return(game_update(state, game, key, key_event));
			break;
	default:
		break;
	}
	return(menu_update(state, key, key_event));
}

enum state input_wiimote(struct game* game, enum state state2) { // state : 0 = MENU, 1 = GAME

	key_event_t key_event;
	SDLKey key;
	enum state state = state2;

	if(wiiuse_poll(wiimotes, MAX_WIIMOTES)) {
		for (int i = 0; i < MAX_WIIMOTES; ++i) {
			//printf("wiimote %d: btns: %d,  held, %d releaded: %d\n", i, wiimotes[i]->btns, wiimotes[i]->btns_held, wiimotes[i]->btns_released);
			if(wiimotes[i]->event == WIIUSE_EVENT) {

				if(IS_JUST_PRESSED(wiimotes[i], WIIMOTE_BUTTON_RIGHT)) {
					switch(i) {
					case 0:
						key = SDLK_UP;
						break;
					case 1:
						key = SDLK_z;
						break;
					case 2:
						key = SDLK_t;
						break;
					case 3:
						key = SDLK_i;
						break;
					}
					key_event = DOWN;
					state = wiimote_send_event(state, game, key, key_event);
				}
				if(IS_JUST_PRESSED(wiimotes[i], WIIMOTE_BUTTON_LEFT)) {
					switch(i) {
					case 0:
						key = SDLK_DOWN;
						break;
					case 1:
						key = SDLK_s;
						break;
					case 2:
						key = SDLK_g;
						break;
					case 3:
						key = SDLK_k;
						break;
					}
					key_event = DOWN;
					state = wiimote_send_event(state, game, key, key_event);
				}
				if(IS_JUST_PRESSED(wiimotes[i], WIIMOTE_BUTTON_UP)) {
					switch(i) {
					case 0:
						key = SDLK_LEFT;
						break;
					case 1:
						key = SDLK_q;
						break;
					case 2:
						key = SDLK_f;
						break;
					case 3:
						key = SDLK_j;
						break;
					}
					key_event = DOWN;
					state = wiimote_send_event(state, game, key, key_event);
				}
				if(IS_JUST_PRESSED(wiimotes[i], WIIMOTE_BUTTON_DOWN)) {
					switch(i) {
					case 0:
						key = SDLK_RIGHT;
						break;
					case 1:
						key = SDLK_d;
						break;
					case 2:
						key = SDLK_h;
						break;
					case 3:
						key = SDLK_l;
						break;
					}
					key_event = DOWN;
					state = wiimote_send_event(state, game, key, key_event);
				}
				if(IS_JUST_PRESSED(wiimotes[i], WIIMOTE_BUTTON_ONE)) {
					switch(i) {
					case 0:
						key = SDLK_SPACE;
						break;
					case 1:
						key = SDLK_a;
						break;
					case 2:
						key = SDLK_r;
						break;
					case 3:
						key = SDLK_u;
						break;
					}
					key_event = DOWN;
					state = wiimote_send_event(state, game, key, key_event);
				}
				if(IS_JUST_PRESSED(wiimotes[0], WIIMOTE_BUTTON_TWO)) {
					key = SDLK_RETURN;
					key_event = DOWN;
					state = wiimote_send_event(state, game, key, key_event);
				}
				if(IS_JUST_PRESSED(wiimotes[0], WIIMOTE_BUTTON_HOME)) {
					key = SDLK_ESCAPE;
					key_event = DOWN;
					state = wiimote_send_event(state, game, key, key_event);
				}

				if(IS_RELEASED(wiimotes[i], WIIMOTE_BUTTON_RIGHT)) {
					switch(i) {
					case 0:
						key = SDLK_UP;
						break;
					case 1:
						key = SDLK_z;
						break;
					case 2:
						key = SDLK_t;
						break;
					case 3:
						key = SDLK_i;
						break;
					}
					key_event = UP;
					state = wiimote_send_event(state, game, key, key_event);
				}
				if(IS_RELEASED(wiimotes[i], WIIMOTE_BUTTON_LEFT)) {
					switch(i) {
					case 0:
						key = SDLK_DOWN;
						break;
					case 1:
						key = SDLK_s;
						break;
					case 2:
						key = SDLK_g;
						break;
					case 3:
						key = SDLK_k;
						break;
					}
					key_event = UP;
					state = wiimote_send_event(state, game, key, key_event);
				}
				if(IS_RELEASED(wiimotes[i], WIIMOTE_BUTTON_UP)) {
					switch(i) {
					case 0:
						key = SDLK_LEFT;
						break;
					case 1:
						key = SDLK_q;
						break;
					case 2:
						key = SDLK_f;
						break;
					case 3:
						key = SDLK_j;
						break;
					}
					key_event = UP;
					state = wiimote_send_event(state, game, key, key_event);
				}
				if(IS_RELEASED(wiimotes[i], WIIMOTE_BUTTON_DOWN)) {
					switch(i) {
					case 0:
						key = SDLK_RIGHT;
						break;
					case 1:
						key = SDLK_d;
						break;
					case 2:
						key = SDLK_h;
						break;
					case 3:
						key = SDLK_l;
						break;
					}
					key_event = UP;
					state = wiimote_send_event(state, game, key, key_event);
				}
			}
		}
	}
	return state;
}

// id = {1 2 3 4}, state = {0 1}
void wiimote_set_rumble(int id, int state) {
	assert(id >= 1 && id <= MAX_WIIMOTES+1 && (state == 0 || state == 1));
	wiiuse_rumble(wiimotes[id-1], state);
}
#endif
