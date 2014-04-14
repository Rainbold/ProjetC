#include <assert.h>
#include <SDL/SDL.h>
#include <game.h>
#include <misc.h>
#include <window.h>
#include <sprite.h>
#include <constant.h>
#include <menu.h>

#define MAX_FIELD_MENU		3

struct menu {
	int nb_field;
	int pos;
	enum select_menu background;
	SDL_Surface* selector;

	struct menu* sup_menu;
	struct menu* sub_menu1;
	struct menu* sub_menu2;

	SDL_Surface* header;
	int pos_header_x;
	int pos_header_y;

	enum select_menu select[MAX_FIELD_MENU];
	int	select_x[MAX_FIELD_MENU];
	int select_y[MAX_FIELD_MENU];
	int pos_select_x[MAX_FIELD_MENU];
	int pos_select_y[MAX_FIELD_MENU];
};

struct menu* g_menu;
struct menu* m_menu;

struct menu* new_menu(enum type_menu type, struct menu* sup_menu) {
	struct menu* menu = malloc(sizeof(*menu));

	switch(type) {
	case MAIN:
		menu->nb_field = 2;
		menu->pos = 0;
		menu->background = M_BG_MAINMENU;
		menu->selector = sprite_get_menu(M_SELECT);

		menu->sup_menu = NULL;
		menu->sub_menu1 = new_menu(SINGLE, menu);
		menu->sub_menu2 = new_menu(MULTI, menu);

		menu->header = NULL;

		menu->select[0] = M_B_SINGLE;
		menu->select[1] = M_B_MULTI;
		menu->select_x[0] = 70;
		menu->select_y[0] = -120;
		menu->select_x[1] = 70;
		menu->select_y[1] = -90;
		menu->pos_select_x[0] = 55;
		menu->pos_select_y[0] = -120;
		menu->pos_select_x[1] = 55;
		menu->pos_select_y[1] = -90;
		g_menu = menu;
		m_menu = menu;
		break;

	case SINGLE:
		menu->nb_field = 2;
		menu->pos = 0;
		menu->background = M_BG_MAINMENU;
		menu->selector = sprite_get_menu(M_SELECT);

		menu->header = NULL;

		menu->sup_menu = sup_menu;
		menu->sub_menu1 = NULL;
		menu->sub_menu2 = NULL;
		menu->select[0] = M_B_NEWGAME;
		menu->select[1] = M_B_LOADGAME;
		menu->select_x[0] = 70;
		menu->select_y[0] = -120;
		menu->select_x[1] = 70;
		menu->select_y[1] = -90;
		menu->pos_select_x[0] = 55;
		menu->pos_select_y[0] = -120;
		menu->pos_select_x[1] = 55;
		menu->pos_select_y[1] = -90;
		break;

	case MULTI:
		menu->nb_field = 3;
		menu->pos = 0;
		menu->background = M_BG_MAINMENU;
		menu->selector = sprite_get_menu(M_SELECT);

		menu->sup_menu = sup_menu;
		menu->sub_menu1 = NULL;
		menu->sub_menu2 = NULL;

		menu->header = NULL;

		menu->select[0] = M_B_2PLAYER;
		menu->select[1] = M_B_3PLAYER;
		menu->select[2] = M_B_4PLAYER;
		menu->select_x[0] = 70;
		menu->select_y[0] = -120;
		menu->select_x[1] = 70;
		menu->select_y[1] = -90;
		menu->select_x[2] = 70;
		menu->select_y[2] = -60;
		menu->pos_select_x[0] = 55;
		menu->pos_select_y[0] = -120;
		menu->pos_select_x[1] = 55;
		menu->pos_select_y[1] = -90;
		menu->pos_select_x[2] = 55;
		menu->pos_select_y[2] = -60;
		break;

	case PAUSE:
		menu->nb_field = 3;
		menu->pos = 0;
		menu->background = M_BG_GREY;
		menu->selector = sprite_get_menu(M_SELECT);

		menu->sup_menu = NULL;
		menu->sub_menu1 = NULL;//new_menu(SAVE, menu);
		menu->sub_menu2 = NULL;

		menu->header = sprite_get_menu(M_H_PAUSE);

		menu->select[0] = M_B_KEEP;
		menu->select[1] = M_B_MAINMENU;
		menu->select[2] = M_B_QUIT;
		// Relative position from center
		menu->pos_header_x = 0;
		menu->pos_header_y = -100;
		menu->select_x[0] = 0;
		menu->select_y[0] = -30;
		menu->select_x[1] = 0;
		menu->select_y[1] = 0;
		menu->select_x[2] = 0;
		menu->select_y[2] = 30;
		menu->pos_select_x[0] = -15;
		menu->pos_select_y[0] = -30;
		menu->pos_select_x[1] = -15;
		menu->pos_select_y[1] = 0;
		menu->pos_select_x[2] = -15;
		menu->pos_select_y[2] = 30;
		g_menu = menu;
		m_menu = menu;
		break;

	case SAVE: // todo
/*		menu->nb_field = 2;
		menu->pos = 0;
		menu->background = M_BG_MAINMENU;
		menu->selector = sprite_get_menu(M_SELECT);

		menu->sup_menu = NULL;
		menu->sub_menu1 = new_menu(SINGLE);
		menu->sub_menu2 = new_menu(MULTI);

		menu->select[0] = M_B_SINGLE;
		menu->select[1] = M_B_MULTI;
		menu->select_x[0] = 370;
		menu->select_y[0] = 180;
		menu->select_x[1] = 370;
		menu->select_y[1] = 210;
		menu->pos_select_x[0] = 355;
		menu->pos_select_y[0] = 180;
		menu->pos_select_x[1] = 355;
		menu->pos_select_y[1] = 210;
		break;
*/
	default:
		menu = NULL;
		break;
	}
	return menu;
}

enum state menu_update(enum state state, int key, key_event_t key_event) {
	if(g_menu != NULL) {
		if(key_event == DOWN) {
			switch (key) {
			case SDLK_RETURN:
			case SDLK_KP_ENTER:
				switch(g_menu->select[g_menu->pos]) {
				case M_B_SINGLE:
					g_menu = g_menu->sub_menu1;
					break;
				case M_B_NEWGAME:
					return NEWGAME_SINGLE;
					break;
				case M_B_LOADGAME:
					//return LOADGAME; todo
					break;

				case M_B_MULTI:
					g_menu = g_menu->sub_menu2;
					return MENU;
					break;
				case M_B_2PLAYER:
					//return NEWGAME_MULTI2; todo multi2
					break;
				case M_B_3PLAYER:
					//return NEWGAME_MULTI3; todo multi3
					break;
				case M_B_4PLAYER:
					//return NEWGAME_MULTI4; todo multi4
					break;

				case M_B_KEEP:
					return KEEP;
					break;
				case M_B_MAINMENU:
					//g_menu = g_menu->sub_menu1; todo save
					break;
				case M_B_QUIT:
					//g_menu = g_menu->sub_menu1; todo save
					break;
				default:
					break;
				}
				break;

				case SDLK_UP:
					if(g_menu->pos > 0){
						g_menu->pos--;
					}
					break;
				case SDLK_DOWN:
					if(g_menu->pos < g_menu->nb_field - 1){
						g_menu->pos++;
					}
					break;
				case SDLK_ESCAPE:
					if(g_menu->sup_menu == NULL)
						return QUIT;
					else
						g_menu = g_menu->sup_menu;
					break;
				default:
					break;
			}
		}
	}
	return state;
}

void menu_display(int mid_w, int mid_h, enum type_menu type) {
	if(g_menu != NULL) {

		if(type != PAUSE)
			window_clear();

		window_display_image(	sprite_get_menu(g_menu->background),
								0,
								0);

		if(g_menu->header != NULL)
			window_display_image(	g_menu->header,
									mid_w + g_menu->pos_header_x,
									mid_h + g_menu->pos_header_y);

		for (int i = 0; i < g_menu->nb_field; i++)
			window_display_image(	sprite_get_menu(g_menu->select[i]),
									mid_w + g_menu->select_x[i],
									mid_h + g_menu->select_y[i]);

		window_display_image(	g_menu->selector,
								mid_w + g_menu->pos_select_x[g_menu->pos],
								mid_h + g_menu->pos_select_y[g_menu->pos]);
		if(type != PAUSE)
			window_refresh();
	}
}

// send NULL if you want to free the global menu
void menu_free(struct menu* menu) {
	struct menu* m;
	if(menu ==  NULL)
		m = m_menu;
	else
		m = menu;

	if(m == NULL)
		return;

	if(m->sub_menu1 != NULL)
		menu_free(m->sub_menu1);

	if(m->sub_menu2 != NULL)
		menu_free(m->sub_menu2);

	free(m);

	if(m == m_menu) {
		m_menu = NULL;
		g_menu = NULL;
	}
}

