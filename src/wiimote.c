#include <stdlib.h>
#include <stdio.h>
#include <wiiuse.h>
#include <constant.h>
#include <SDL/SDL.h>
#include <assert.h>
#include <game.h>
#include <menu.h>

#ifdef USE_WIIMOTE

#define MAX_WIIMOTES 1
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
//	wiiuse_set_leds(wiimotes[1], WIIMOTE_LED_2);
//	wiiuse_set_leds(wiimotes[2], WIIMOTE_LED_3);
//	wiiuse_set_leds(wiimotes[3], WIIMOTE_LED_4);

	wiiuse_rumble(wiimotes[0], 1);
//	wiiuse_rumble(wiimotes[1], 1);
//	wiiuse_rumble(wiimotes[2], 1);
//	wiiuse_rumble(wiimotes[3], 1);

	SDL_Delay(200);

	wiiuse_rumble(wiimotes[0], 0);
//	wiiuse_rumble(wiimotes[1], 0);
//	wiiuse_rumble(wiimotes[2], 0);
//	wiiuse_rumble(wiimotes[3], 0);
}


enum state input_wiimote(struct game* game, enum state state) { // state : 0 = MENU, 1 = GAME

	key_event_t key_event;
	SDLKey key;
	int send = 0;

	if(wiiuse_poll(wiimotes, MAX_WIIMOTES)) {
	int i = 0;
		printf("ok\n");
//		for (int i = 0; i < MAX_WIIMOTES; ++i) {
			send = 0;
			printf("wiimote: %d %d %d\n", wiimotes[i]->btns, wiimotes[i]->btns_held, wiimotes[i]->btns_released);
			if(wiimotes[i]->event == WIIUSE_EVENT) {

				if(IS_JUST_PRESSED(wiimotes[i], WIIMOTE_BUTTON_RIGHT)) {
					key = SDLK_UP;
					key_event = DOWN;
					send = 1;
				}
				if(IS_JUST_PRESSED(wiimotes[i], WIIMOTE_BUTTON_LEFT)) {
					key = SDLK_DOWN;
					key_event = DOWN;
					send = 1;
				}
				if(IS_JUST_PRESSED(wiimotes[i], WIIMOTE_BUTTON_UP)) {
					key = SDLK_LEFT;
					key_event = DOWN;
					send = 1;
				}
				if(IS_JUST_PRESSED(wiimotes[i], WIIMOTE_BUTTON_DOWN)) {
					key = SDLK_RIGHT;
					key_event = DOWN;
					send = 1;
				}
				if(IS_JUST_PRESSED(wiimotes[i], WIIMOTE_BUTTON_ONE)) {
					key = SDLK_SPACE;
					key_event = DOWN;
					send = 1;
				}
				if(IS_JUST_PRESSED(wiimotes[i], WIIMOTE_BUTTON_TWO)) {
					key = SDLK_RETURN;
					key_event = DOWN;
					send = 1;
				}
				if(IS_JUST_PRESSED(wiimotes[i], WIIMOTE_BUTTON_HOME)) {
					key = SDLK_ESCAPE;
					key_event = DOWN;
					send = 1;
				}

				if(IS_RELEASED(wiimotes[i], WIIMOTE_BUTTON_RIGHT)) {
					key = SDLK_UP;
					key_event = UP;
					send = 1;
				}
				if(IS_RELEASED(wiimotes[i], WIIMOTE_BUTTON_LEFT)) {
					key = SDLK_DOWN;
					key_event = UP;
					send = 1;
				}
				if(IS_RELEASED(wiimotes[i], WIIMOTE_BUTTON_UP)) {
					key = SDLK_LEFT;
					key_event = UP;
					send = 1;
				}
				if(IS_RELEASED(wiimotes[i], WIIMOTE_BUTTON_DOWN)) {
					key = SDLK_RIGHT;
					key_event = UP;
					send = 1;
				}
			}
			if(send) {
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
	//	}
	}
	return state;
}

// id = {1 2 3 4}, state = {0 1}
void wiimote_set_rumble(int id, int state) {
	assert(id >= 1 && id <= MAX_WIIMOTES+1 && (state == 0 || state == 1));
	wiiuse_rumble(wiimotes[id-1], state);
}
#endif
