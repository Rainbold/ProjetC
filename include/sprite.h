#ifndef SPRITE_H_
#define SPRITE_H_

#include <SDL/SDL.h>
#include <map.h>
#include <constant.h>
 
// load game's sprites
void sprite_load();

SDL_Surface* sprite_get_players(int id);
SDL_Rect sprite_get_rect_player_anim(int i, int id, enum way direction);

SDL_Surface* sprite_get_map_multi(int i);
int sprite_get_nb_map_multi();
int sprite_get_max_width();

SDL_Surface* sprite_get_monster(m_type type, way_t direction);

SDL_Surface* sprite_get_bonus(bonus_type_t bonus_type);

SDL_Surface* sprite_get_menu(enum select_menu select_menu);
SDL_Surface* sprite_get_score(int i);
SDL_Rect* sprite_get_rect_stars();

SDL_Surface* sprite_get_grey();
SDL_Surface* sprite_get_tree();
SDL_Surface* sprite_get_box();
SDL_Surface* sprite_get_key();
SDL_Surface* sprite_get_stone();
SDL_Surface* sprite_get_door();
SDL_Surface* sprite_get_closed_door();
SDL_Surface* sprite_get_flag();
SDL_Surface* sprite_get_princess();
SDL_Surface* sprite_get_empty();
SDL_Surface* sprite_get_wall_b();
SDL_Surface* sprite_get_wall_g();
SDL_Surface* sprite_get_wall_h();
SDL_Surface* sprite_get_wall_d();
SDL_Surface* sprite_get_angle_bg();
SDL_Surface* sprite_get_angle_bd();
SDL_Surface* sprite_get_angle_hg();
SDL_Surface* sprite_get_angle_hd();
SDL_Surface* sprite_get_bloc();

SDL_Surface* sprite_get_number(short number);
SDL_Surface* sprite_get_number_white(short number);

SDL_Surface* sprite_get_banner_line();
SDL_Surface* sprite_get_banner_bomb();
SDL_Surface* sprite_get_banner_range();
SDL_Surface* sprite_get_banner_life();

SDL_Surface* sprite_get_bombs();

SDL_Rect sprite_get_rect_bomb_anim(int i, int j);

void sprite_free();

#endif /* SPRITE_H_ */
