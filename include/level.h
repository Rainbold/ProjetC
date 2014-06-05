#ifndef LEVEL_H_
#define LEVEL_H_

#include <game.h>
#include <map.h>
#include <player.h>


struct player;
struct level;

// return level number n_lvl and start with the n_map map
struct level* level_get_level(struct game* game, int n_lvl, int n_map);

// Change level
void level_change_level(struct game* game, struct player* player, struct map* map);

// Change map
void level_change_map(struct game* game,  struct player* player, struct map* map, unsigned char num);

// return current map of the level
struct map* level_get_curr_map(struct level* level);

// Set cur_map
void level_set_cur_map(struct level* level, int i);

// Return cur_map
int level_get_curr_nb_map(struct level* level);

// Set a map in the array maps
void level_set_cur_map_struct(struct level* level, int cur_map, struct map* map);

// return map number num of the level
struct map*  level_get_map(struct level* level, int num);

// free the level
void  level_free(struct level* level);

// display the level
void  level_display(struct level* level);

// free level struct
void   level_free(struct level* level);

#endif /* LEVEL_H_ */
