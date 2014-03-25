#ifndef SPRITE_H_
#define SPRITE_H_

#include <SDL/SDL.h>
#include <map.h>
#include <constant.h>
 
// load game's sprites
void		 sprite_load();

SDL_Surface* sprite_get_bomb(short bombAnim);
SDL_Surface* sprite_get_player(way_t direction);
SDL_Surface* sprite_get_monster(way_t direction);
SDL_Surface* sprite_get_bonus(bonus_type_t bonus_type);
SDL_Surface* sprite_get_menu(enum select_menu select_menu);
SDL_Surface* sprite_get_grey();
SDL_Surface* sprite_get_tree();
SDL_Surface* sprite_get_box();
SDL_Surface* sprite_get_key();
SDL_Surface* sprite_get_stone();
SDL_Surface* sprite_get_door();
SDL_Surface* sprite_get_closed_door();

SDL_Surface* sprite_get_number(short number);
SDL_Surface* sprite_get_banner_life();
SDL_Surface* sprite_get_banner_bomb();
SDL_Surface* sprite_get_banner_line();
SDL_Surface* sprite_get_banner_range();

SDL_Surface* sprite_get_sprite_bomb();

SDL_Rect sprite_get_br_wait(int i);
SDL_Rect sprite_get_br_center(int i);
SDL_Rect sprite_get_br_down(int i);
SDL_Rect sprite_get_br_h(int i);
SDL_Rect sprite_get_br_left(int i);
SDL_Rect sprite_get_br_right(int i);
SDL_Rect sprite_get_br_up(int i);
SDL_Rect sprite_get_br_v(int i);

#endif /* SPRITE_H_ */
