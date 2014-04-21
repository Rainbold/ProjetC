#include <assert.h>
#include <SDL/SDL.h>
#include <game.h>
#include <misc.h>
#include <window.h>
#include <sprite.h>
#include <constant.h>
#include <menu.h>

#define MAX_FIELD_MENU 20

struct animation{
	int x, y, offset;
	SDL_Surface* surface;
	SDL_Rect* rect;
};

struct menu {
	enum type_menu type;
	int nb_field;
	int nb_sub[MAX_FIELD_MENU];
	int pos_field, pos_sub;

	/*	   -       -  nb_field	// 0
	 * 	   |       |  nb_sub1	// 1
	 * 	   |       |  nb_sub2	// 2
	 *     |       |  nb_sub3	// 3
	 */

	enum select_menu select[MAX_FIELD_MENU][MAX_FIELD_MENU + 1];

	enum select_menu header;
	int pos_header_x;
	int pos_header_y;

	int	pos_select_x;
	int pos_select_y[MAX_FIELD_MENU];

	enum select_menu selector;
	int selector_offset;

	enum select_menu background;
	int pos_bg_x, pos_bg_y;

	int anim;

	int nb_anim;
	struct animation** anims;
};

struct menu* g_menu;

void new_menu(enum type_menu type) {
	struct menu* menu = malloc(sizeof(*menu));;

	switch(type) {
	case MAIN:
		menu->type = MAIN;
		menu->nb_field = 5;
		menu->nb_sub[0] = 2;
		menu->nb_sub[1] = 0;
		menu->nb_sub[2] = 0;
		menu->nb_sub[3] = 0;
		menu->nb_sub[4] = 0;

		menu->pos_field = 0;
		menu->pos_sub = 0;

		menu->select[0][0] = M_B_SINGLE;
		menu->select[0][1] = M_B_NEWGAME;
		menu->select[0][2] = M_B_LOADGAME;

		menu->select[1][0] = M_B_2PLAYER;

		int cont = M_B_2PLAYER_MAP_1;
		int i = 0;
		while(sprite_get_menu(cont) && i<=10)
		{
			i++;
			menu->nb_sub[1] = i;
			menu->select[1][i] = cont;
			cont++;
		}

		cont = M_B_3PLAYER_MAP_1;
		i = 0;
		while(sprite_get_menu(cont) && i<=10)
		{
			i++;
			menu->nb_sub[2] = i;
			menu->select[2][i] = cont;
			cont++;
		}

		cont = M_B_4PLAYER_MAP_1;
		i = 0;
		while(sprite_get_menu(cont) && i<=10)
		{
			i++;
			menu->nb_sub[3] = i;
			menu->select[3][i] = cont;
			cont++;
		}

		menu->select[2][0] = M_B_3PLAYER;
		menu->select[3][0] = M_B_4PLAYER;

		menu->select[4][0] = M_B_QUIT;

		menu->header = -1;
		menu->pos_header_x = 0;
		menu->pos_header_y = 0;

		menu->pos_select_x = 70;
		menu->pos_select_y[0] = -120;
		menu->pos_select_y[1] = -90;
		menu->pos_select_y[2] = -60;
		menu->pos_select_y[3] = -30;
		menu->pos_select_y[4] = 0;
		menu->pos_select_y[5] = 30;
		menu->pos_select_y[6] = 60;
		menu->pos_select_y[7] = 90;
		menu->pos_select_y[8] = 120;
		menu->pos_select_y[9] = 150;
		menu->pos_select_y[10] = 180;

		menu->selector = M_SELECT;
		menu->selector_offset = -15;

		menu->background = M_BG_MAINMENU;
		menu->pos_bg_x = -300;
		menu->pos_bg_y = -300;

		menu->anim = 0;

		menu->nb_anim = 12;

		menu->anims = malloc(sizeof(*menu->anims) * menu->nb_anim);

		// Common data
		for(int i = 0; i < menu->nb_anim; i++) {
			menu->anims[i] = malloc(sizeof(struct animation));
			menu->anims[i]->offset = 2*i;
			menu->anims[i]->surface = sprite_get_menu(M_STARS);
			menu->anims[i]->rect = sprite_get_rect_stars();
		}

		menu->anims[0]->x = -250;
		menu->anims[0]->y = -270;

		menu->anims[1]->x = 25;
		menu->anims[1]->y = -260;

		menu->anims[2]->x = -60;
		menu->anims[2]->y = -60;

		menu->anims[3]->x = -270;
		menu->anims[3]->y = -125;

		menu->anims[4]->x = 250;
		menu->anims[4]->y = -30;

		menu->anims[5]->x = -15;
		menu->anims[5]->y = -110;

		menu->anims[6]->x = -160;
		menu->anims[6]->y = -140;

		menu->anims[7]->x = 125;
		menu->anims[7]->y = -20;

		menu->anims[8]->x = 185;
		menu->anims[8]->y = -155;

		menu->anims[9]->x = 180;
		menu->anims[9]->y = -275;

		menu->anims[10]->x = -115;
		menu->anims[10]->y = -280;

		menu->anims[11]->x = -275;
		menu->anims[11]->y = -20;
		g_menu = menu;
		break;

	case PAUSE_SINGLE:
		menu->type = PAUSE_SINGLE;
		menu->nb_field = 3;
		menu->nb_sub[0] = 0;
		menu->nb_sub[1] = 2;
		menu->nb_sub[2] = 2;

		menu->pos_field = 0;
		menu->pos_sub = 0;

		menu->select[0][0] = M_B_KEEP;

		menu->select[1][0] = M_B_MAINMENU;
		menu->select[1][1] = M_B_YES;
		menu->select[1][2] = M_B_NO;

		menu->select[2][0] = M_B_QUIT;
		menu->select[2][1] = M_B_YES;
		menu->select[2][2] = M_B_NO;

		menu->header = M_H_PAUSE;
		menu->pos_header_x = -80;
		menu->pos_header_y = -100;

		menu->pos_select_x = -75;
		menu->pos_select_y[0] = -30;
		menu->pos_select_y[1] = 0;
		menu->pos_select_y[2] = 30;


		menu->selector = M_SELECT;
		menu->selector_offset = -15;

		menu->background = M_BG_GREY;
		menu->pos_bg_x = -240;
		menu->pos_bg_y = -240;

		menu->anim = 0;

		menu->nb_anim = 0;

		menu->anims = NULL;

		g_menu = menu;
		break;

	default:
		break;
	}
}

enum state menu_update(enum state state, int key, key_event_t key_event) {
	if(g_menu != NULL) {
		if(key_event == DOWN) {
			switch (key) {
			case SDLK_RETURN:
			case SDLK_KP_ENTER:
				switch(g_menu->select[g_menu->pos_field][g_menu->pos_sub]) {

				case M_B_2PLAYER:
				case M_B_3PLAYER:
				case M_B_4PLAYER:
				case M_B_SINGLE:
				case M_B_MULTI:
					g_menu->pos_sub = 1;
					break;
				case M_B_MAINMENU:
					if(g_menu->type == PAUSE_SINGLE) {
						g_menu->pos_sub = 1;
						g_menu->header = M_H_SAVE;
					}
					break;

				case M_B_QUIT:
					if(g_menu->type == PAUSE_SINGLE) {
						g_menu->pos_sub = 1;
						g_menu->header = M_H_SAVE;
					}
					else
						return QUIT;
					break;

				case M_B_NEWGAME:
					return NEWGAME_SINGLE;
					break;
				case M_B_LOADGAME:
					//return LOADGAME; todo
					break;
				/*
				case M_B_2PLAYER:
					return NEWGAME_MULTI2;
					break;
				case M_B_3PLAYER:
					return NEWGAME_MULTI3;
					break;
				case M_B_4PLAYER:
					return NEWGAME_MULTI4;
				*/

				case M_B_2PLAYER_MAP_1:
					return NEWGAME_MULTI2_1;
					break;
				case M_B_2PLAYER_MAP_2:
					return NEWGAME_MULTI2_2;
					break;
				case M_B_2PLAYER_MAP_3:
					return NEWGAME_MULTI2_3;
					break;
				case M_B_2PLAYER_MAP_4:
					return NEWGAME_MULTI2_4;
					break;
				case M_B_2PLAYER_MAP_5:
					return NEWGAME_MULTI2_5;
					break;
				case M_B_2PLAYER_MAP_6:
					return NEWGAME_MULTI2_6;
					break;
				case M_B_2PLAYER_MAP_7:
					return NEWGAME_MULTI2_7;
					break;
				case M_B_2PLAYER_MAP_8:
					return NEWGAME_MULTI2_8;
					break;
				case M_B_2PLAYER_MAP_9:
					return NEWGAME_MULTI2_9;
					break;
				case M_B_2PLAYER_MAP_10:
					return NEWGAME_MULTI2_10;
					break;

				case M_B_3PLAYER_MAP_1:
					return NEWGAME_MULTI3_1;
					break;
				case M_B_3PLAYER_MAP_2:
					return NEWGAME_MULTI3_2;
					break;
				case M_B_3PLAYER_MAP_3:
					return NEWGAME_MULTI3_3;
					break;
				case M_B_3PLAYER_MAP_4:
					return NEWGAME_MULTI3_4;
					break;
				case M_B_3PLAYER_MAP_5:
					return NEWGAME_MULTI3_5;
					break;
				case M_B_3PLAYER_MAP_6:
					return NEWGAME_MULTI3_6;
					break;
				case M_B_3PLAYER_MAP_7:
					return NEWGAME_MULTI3_7;
					break;
				case M_B_3PLAYER_MAP_8:
					return NEWGAME_MULTI3_8;
					break;
				case M_B_3PLAYER_MAP_9:
					return NEWGAME_MULTI3_9;
					break;
				case M_B_3PLAYER_MAP_10:
					return NEWGAME_MULTI3_10;
					break;

				case M_B_4PLAYER_MAP_1:
					return NEWGAME_MULTI4_1;
					break;
				case M_B_4PLAYER_MAP_2:
					return NEWGAME_MULTI4_2;
					break;
				case M_B_4PLAYER_MAP_3:
					return NEWGAME_MULTI4_3;
					break;
				case M_B_4PLAYER_MAP_4:
					return NEWGAME_MULTI4_4;
					break;
				case M_B_4PLAYER_MAP_5:
					return NEWGAME_MULTI4_5;
					break;
				case M_B_4PLAYER_MAP_6:
					return NEWGAME_MULTI4_6;
					break;
				case M_B_4PLAYER_MAP_7:
					return NEWGAME_MULTI4_7;
					break;
				case M_B_4PLAYER_MAP_8:
					return NEWGAME_MULTI4_8;
					break;
				case M_B_4PLAYER_MAP_9:
					return NEWGAME_MULTI4_9;
					break;
				case M_B_4PLAYER_MAP_10:
					return NEWGAME_MULTI4_10;
					break;

				case M_B_KEEP:
					return KEEP;
					break;

				case M_B_YES:
					// todo save
					break;
				case M_B_NO:
					switch(g_menu->select[g_menu->pos_field][0]) {
					case M_B_MAINMENU:
					return MAINMENU;
						break;
					case M_B_QUIT:
						return QUIT;
						break;
					default:
						break;
					}
					break;

				default:
					break;
				}
				break;

				case SDLK_UP:
					if(g_menu->pos_sub == 0) {
						if(g_menu->pos_field > 0)
							g_menu->pos_field--;
					}
					else {
						if(g_menu->pos_sub > 1)
							g_menu->pos_sub--;
					}
					break;

				case SDLK_DOWN:
					if(g_menu->pos_sub == 0) {
						if(g_menu->pos_field < g_menu->nb_field - 1)
							g_menu->pos_field++;
					}
					else {
						if(g_menu->pos_sub < g_menu->nb_sub[g_menu->pos_field])
							g_menu->pos_sub++;
					}
					break;

				case SDLK_ESCAPE:
					if(g_menu->pos_sub != 0)
						g_menu->pos_sub = 0;
					else
						return QUIT;
					break;
				default:
					break;
			}
		}
	}
	return state;
}

void menu_display(int mid_w, int mid_h) {
	if(g_menu != NULL) {

		if(g_menu->type == MAIN)
			window_clear();

		window_display_image(	sprite_get_menu(g_menu->background),
								mid_w + g_menu->pos_bg_x,
								mid_h + g_menu->pos_bg_y);

		if(g_menu->header != -1)
			window_display_image(	sprite_get_menu(g_menu->header),
									mid_w + g_menu->pos_header_x,
									mid_h + g_menu->pos_header_y);

		if(g_menu->pos_sub == 0) {
			for(int i = 0; i < g_menu->nb_field; i++)
				window_display_image(	sprite_get_menu(g_menu->select[i][0]),
										mid_w + g_menu->pos_select_x,
										mid_h + g_menu->pos_select_y[i]);
			window_display_image(	sprite_get_menu(g_menu->selector),
									mid_w + g_menu->pos_select_x + g_menu->selector_offset,
									mid_h + g_menu->pos_select_y[g_menu->pos_field]);
		}
		else {
			for(int i = 0; i < g_menu->nb_sub[g_menu->pos_field]; i++) {
							window_display_image(	sprite_get_menu(g_menu->select[g_menu->pos_field][i+1]),
													mid_w + g_menu->pos_select_x,
													mid_h + g_menu->pos_select_y[i]);
			}
			window_display_image(	sprite_get_menu(g_menu->selector),
									mid_w + g_menu->pos_select_x + g_menu->selector_offset,
									mid_h + g_menu->pos_select_y[g_menu->pos_sub-1]);
		}

		for(int i = 0; i < g_menu->nb_anim; i++)
			if((g_menu->anim/2 + g_menu->anims[i]->offset)%12 - 5 >= 0)
				window_display_sprite(	g_menu->anims[i]->surface,
									g_menu->anims[i]->rect[(g_menu->anim/2 + g_menu->anims[i]->offset)%12 -5],
									mid_w + g_menu->anims[i]->x,
									mid_h + g_menu->anims[i]->y);

		if(g_menu->type == MAIN)
			window_refresh();

		g_menu->anim++;
	}
}

// send NULL if you want to free the global menu
void menu_free() {
	if(g_menu != NULL) {
		if(g_menu->anims != NULL) {
			for(int i = 0; i < g_menu->nb_anim; i++)
				if(g_menu->anims[i] != NULL)
					free(g_menu->anims[i]);
			free(g_menu->anims);
		}
		free(g_menu);
		g_menu = NULL;
	}
}

