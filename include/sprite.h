#ifndef SPRITE_H_
#define SPRITE_H_

#include <SDL/SDL.h>
#include <map.h>
#include <constant.h>
 
// load game's sprites
void sprite_load();

// Return the sprite sheet of a player
SDL_Surface* sprite_get_players(int id);

// Return the rect of a player
SDL_Rect sprite_get_rect_player_anim(int i, int id, enum way direction);

// Return the names of the multiplayer maps
SDL_Surface* sprite_get_map_multi(int i);

// Return the number of multiplayer map
int sprite_get_nb_map_multi();

// Return the max width of multiplayer maps
int sprite_get_max_width();

// Return the surface of a monster
SDL_Surface* sprite_get_monster(m_type type, way_t direction);

// Return the surface of a bonus
SDL_Surface* sprite_get_bonus(bonus_type_t bonus_type);

// Return a surface of a menu
SDL_Surface* sprite_get_menu(enum select_menu select_menu);

// Return the surface of score
SDL_Surface* sprite_get_score(int i);

// Return the rect of stars
SDL_Rect* sprite_get_rect_stars();

// Return suface
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

// Return the surface of a number
SDL_Surface* sprite_get_number(short number);
SDL_Surface* sprite_get_number_white(short number);

// Return surfaces for the banner
SDL_Surface* sprite_get_banner_line();
SDL_Surface* sprite_get_banner_bomb();
SDL_Surface* sprite_get_banner_range();
SDL_Surface* sprite_get_banner_life();

// Return the bombs sprite sheet
SDL_Surface* sprite_get_bombs();

// Return the rect of bombs
SDL_Rect sprite_get_rect_bomb_anim(int i, int j);

// Free a surface
void sprite_free();

#endif /* SPRITE_H_ */
