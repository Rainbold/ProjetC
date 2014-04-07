#include <assert.h>
#include <SDL/SDL.h>
#include <game.h>
#include <misc.h>
#include <window.h>
#include <sprite.h>
#include <constant.h>
#include <menu.h>

struct menu {
	int nb_field;
	int pos;
	enum select_menu select[MAX_FIELD];
	int	select_x[MAX_FIELD];
	int select_y[MAX_FIELD];
	enum select_menu background;
	SDL_Surface* point;
};

struct menu* new_menu(enum type type){
	struct menu* menu = malloc( sizeof(*menu) );
	switch(type) {
	case MAIN:
		menu->nb_field = 2;
		menu->pos = 0;
		menu->select[0] = M_B_NEWGAME;
		menu->select[1] = M_B_QUIT;
		menu->select_x[0] = 170;
		menu->select_y[0] = 140;
		menu->select_x[1] = 170;
		menu->select_y[1] = 200;
		menu->background = M_BG_MAINMENU;
//		menu->point = sprite_get_player(SOUTH);
		break;
	}
	return menu;
}

enum state menu_update(struct menu* menu, int key, key_event_t key_event) {
	if(key_event == DOWN) {
		switch (key) {
		case SDLK_RETURN:
			switch(menu->select[menu->pos]) {
			case M_B_NEWGAME:
				return NEWGAME1;
				break;
			case M_B_QUIT:
				return QUIT;
				break;
			default:
				break;
			}
			break;
			case SDLK_UP:
				if(menu->pos > 0){
					menu->pos--;
				}
				break;
			case SDLK_DOWN:
				if(menu->pos < menu->nb_field - 1){
					menu->pos++;
				}
				break;
			case SDLK_ESCAPE:
				return QUIT;
				break;
			default:
				break;
		}
	}
	return MENU;
}

void menu_display(struct menu* menu) {
	assert(menu);

	window_clear();
	window_display_image(sprite_get_menu(M_BG_MAINMENU), 0, 0);
	for (int i = 0; i < menu->nb_field; i++) {
		window_display_image(sprite_get_menu(menu->select[i]), menu->select_x[i], menu->select_y[i]);
	}
//	window_display_image(menu->point, menu->select_x[menu->pos] - 40, menu->select_y[menu->pos]);

	window_refresh();

}

void menu_free(struct menu* menu) {
	assert(menu);
	free(menu);
}

